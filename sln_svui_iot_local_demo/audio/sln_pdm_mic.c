/*
 * Copyright 2018, 2020-2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_mic_config.h"

#if (MICS_TYPE == MICS_PDM)

#include "sln_pdm_mic.h"
#include <limits.h>
#include "fsl_dmamux.h"
#include "fsl_sai.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PDM_MIC_MALLOC(x)    pvPortMalloc(x)
#define PDM_MIC_FREE(x)      vPortFree(x)
#define PDM_MIC_DMA_IRQ_PRIO (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)

/* Select Audio/Video PLL (786.48 MHz) as SAI clock source */
#define PDM_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for SAI clock source */
#define PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER (0U) //(1U)
/* Clock divider for SAI clock source */
#define PDM_SAI_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of SAI clock */
#define PDM_SAI_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (PDM_SAI_CLOCK_SOURCE_DIVIDER + 1U) / (PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* Skip first chunks of mic data because mics are not reliable right after boot. */
#define SKIP_DIRTY_FRAMES 4

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Skip first chunks of mic data because mics are not reliable right after boot. */
volatile static uint8_t s_skipDirtyFrames = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void PDM_MIC_DmaCallback(sln_mic_handle_t *handle)
{
    handle->dma->INT |= (1 << handle->dmaChannel);

    handle->dma->TCD[handle->dmaChannel].CSR &= ~DMA_CSR_DONE_MASK;

    handle->dma->TCD[handle->dmaChannel].CSR |= DMA_CSR_ESG_MASK;

    handle->dma->SERQ = DMA_SERQ_SERQ(handle->dmaChannel);

    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

#if ENABLE_AEC
#if USE_MQS
    if (handle->pdmMicUpdateTimestamp != NULL)
    {
        handle->pdmMicUpdateTimestamp();
    }
#endif /* USE_MQS */
#endif /* ENABLE_AEC */

    if (s_skipDirtyFrames > 0)
    {
        s_skipDirtyFrames--;
    }
    else
    {
        if (handle->pingPongTracker & 0x01U)
        {
            if (xEventGroupGetBitsFromISR(handle->eventGroup) & handle->pongFlag)
            {
                xEventGroupSetBitsFromISR((handle->eventGroup), handle->errorFlag, &xHigherPriorityTaskWoken);
            }
            xEventGroupSetBitsFromISR((handle->eventGroup), handle->pongFlag, &xHigherPriorityTaskWoken);
        }
        else
        {
            if (xEventGroupGetBitsFromISR(handle->eventGroup) & handle->pingFlag)
            {
                xEventGroupSetBitsFromISR((handle->eventGroup), handle->errorFlag, &xHigherPriorityTaskWoken);
            }
            xEventGroupSetBitsFromISR((handle->eventGroup), handle->pingFlag, &xHigherPriorityTaskWoken);
        }
    }

    handle->pingPongTracker++;
}

pdm_mic_status_t PDM_MIC_GetSAIConfiguration(uint8_t channelMask,
                                             uint32_t *burstSize,
                                             uint32_t *burstBytes,
                                             uint32_t *startIdx)
{
    // We will assign start index, burt size enum and bust byte count for each channel mapping.
    switch (channelMask)
    {
        // Determine all single channel mapping start index
        case kMicChannel1:
            *startIdx = 0;
            goto set_4byte_burst;
        case kMicChannel2:
            *startIdx = 1;
            goto set_4byte_burst;
        case kMicChannel3:
            *startIdx = 2;
            goto set_4byte_burst;
        case kMicChannel4:
            *startIdx = 3;

        // All byte channel mapping configurations will have these burstSize and burstBytes settings
        set_4byte_burst:
            *burstSize  = kEDMA_TransferSize4Bytes;
            *burstBytes = 4U;
            break;

        // Determine two channel mapping start index
        case kMicChannel1 | kMicChannel2:
            *startIdx = 0;
            goto set_8byte_burst;
        case kMicChannel2 | kMicChannel3:
            *startIdx = 1;
            goto set_8byte_burst;
        case kMicChannel3 | kMicChannel4:
            *startIdx = 2;

        // All two channel mapping configurations will have these burstSize and burstBytes settings
        set_8byte_burst:
            *burstSize = kEDMA_TransferSize8Bytes;
            // burstSize = kEDMA_TransferSize4Bytes;
            *burstBytes = 8U;
            break;

        // Any 3 or 4 channel mic maps
        default:
            *startIdx  = 0;
            *burstSize = kEDMA_TransferSize16Bytes;
            // burstSize = kEDMA_TransferSize4Bytes;
            *burstBytes = 16U;
            break;
    };

    return kPdmMicSuccess;
}

pdm_mic_status_t PDM_MIC_Init(void)
{
    // Coming Soon to A MCU Voice board near you!

    // TODO: Implement driver private memory, set to known values

    return kPdmMicSuccess;
}

pdm_mic_status_t PDM_MIC_ConfigMic(sln_mic_handle_t *handle)
{
    // Map channel mask to DMA transfer config
    uint32_t burstSize  = 0U;
    uint32_t startIndex = 0U;
    uint32_t burstBytes = 0U;

    if (handle->config->sai == SAI1)
    {
        CLOCK_SetMux(kCLOCK_Sai1Mux, PDM_SAI_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Sai1PreDiv, PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
        CLOCK_SetDiv(kCLOCK_Sai1Div, PDM_SAI_CLOCK_SOURCE_DIVIDER);
        CLOCK_EnableClock(kCLOCK_Sai1);
    }
    else if (handle->config->sai == SAI2)
    {
        CLOCK_SetMux(kCLOCK_Sai2Mux, PDM_SAI_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Sai2PreDiv, PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
        CLOCK_SetDiv(kCLOCK_Sai2Div, PDM_SAI_CLOCK_SOURCE_DIVIDER);
        CLOCK_EnableClock(kCLOCK_Sai2);
    }
    else if (handle->config->sai == SAI3)
    {
        CLOCK_SetMux(kCLOCK_Sai3Mux, PDM_SAI_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Sai3PreDiv, PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
        CLOCK_SetDiv(kCLOCK_Sai3Div, PDM_SAI_CLOCK_SOURCE_DIVIDER);
        CLOCK_EnableClock(kCLOCK_Sai3);
    }

    PDM_MIC_GetSAIConfiguration(handle->config->saiChannelMask, &burstSize, &burstBytes, &startIndex);

    handle->dmaTcd[0].SADDR = (uint32_t)(&handle->config->sai->RDR[startIndex]);
    handle->dmaTcd[1].SADDR = (uint32_t)(&handle->config->sai->RDR[startIndex]);

    handle->dmaTcd[0].SOFF = 0U;
    handle->dmaTcd[1].SOFF = 0U;

    handle->dmaTcd[0].ATTR = (DMA_ATTR_SSIZE(burstSize) | DMA_ATTR_DSIZE(burstSize));
    handle->dmaTcd[1].ATTR = (DMA_ATTR_SSIZE(burstSize) | DMA_ATTR_DSIZE(burstSize));

    handle->dmaTcd[0].NBYTES = burstBytes;
    handle->dmaTcd[1].NBYTES = burstBytes;

    handle->dmaTcd[0].SLAST = 0U;
    handle->dmaTcd[1].SLAST = 0U;

    handle->dmaTcd[0].DADDR = (uint32_t)handle->pingPongBuffer[0];
    handle->dmaTcd[1].DADDR = (uint32_t)handle->pingPongBuffer[1];

    handle->dmaTcd[0].DOFF = burstBytes;
    handle->dmaTcd[1].DOFF = burstBytes;

    handle->dmaTcd[0].CITER = handle->config->saiCaptureCount;
    handle->dmaTcd[1].CITER = handle->config->saiCaptureCount;

    handle->dmaTcd[0].DLAST_SGA = (uint32_t)&handle->dmaTcd[1];
    handle->dmaTcd[1].DLAST_SGA = (uint32_t)&handle->dmaTcd[0];

    handle->dmaTcd[0].CSR = (DMA_CSR_INTMAJOR_MASK | DMA_CSR_ESG_MASK);
    handle->dmaTcd[1].CSR = (DMA_CSR_INTMAJOR_MASK | DMA_CSR_ESG_MASK);

    handle->dmaTcd[0].BITER = handle->config->saiCaptureCount;
    handle->dmaTcd[1].BITER = handle->config->saiCaptureCount;

    EDMA_InstallTCD(handle->dma, handle->dmaChannel, &handle->dmaTcd[0]);

    DMAMUX_SetSource(DMAMUX, handle->dmaChannel, handle->dmaRequest);
    DMAMUX_EnableChannel(DMAMUX, handle->dmaChannel);

    NVIC_SetPriority(handle->dmaIrqNum, PDM_MIC_DMA_IRQ_PRIO);
    NVIC_EnableIRQ(handle->dmaIrqNum);

    uint32_t val               = 0;
    uint32_t bclk              = 0;
    uint32_t mclkSourceClockHz = 0U;
    mclkSourceClockHz          = PDM_SAI_CLK_FREQ;
    bclk                       = PDM_MIC_SAI_CLK_HZ;

    handle->config->sai->RCR2 |= I2S_RCR2_BCP_MASK;
    handle->config->sai->RCR3 &= ~I2S_RCR3_WDFL_MASK;

#if USE_NEW_PDM_PCM_LIB
    /* USE_NEW_PDM_PCM_LIB expects PDM data in MSB First format */
    handle->config->sai->RCR4 = I2S_RCR4_MF(1U) | I2S_RCR4_SYWD(31U) | I2S_RCR4_FSE(1U) | I2S_RCR4_FSP(0U) | I2S_RCR4_FRSZ(1U);
#else
    handle->config->sai->RCR4 = I2S_RCR4_MF(0U) | I2S_RCR4_SYWD(31U) | I2S_RCR4_FSE(0U) | I2S_RCR4_FSP(0U) | I2S_RCR4_FRSZ(1U);
#endif /* USE_NEW_PDM_PCM_LIB */

    handle->config->sai->RCR2 |= I2S_RCR2_BCD_MASK;
    handle->config->sai->RCR4 |= I2S_RCR4_FSD_MASK;

    val                       = handle->config->sai->RCR2 & (~I2S_RCR2_MSEL_MASK);
    handle->config->sai->RCR2 = (val | I2S_RCR2_MSEL(kSAI_BclkSourceMclkDiv));
    val                       = 0;

    val = handle->config->sai->RCR2;
    val &= ~I2S_RCR2_SYNC_MASK;
    handle->config->sai->RCR2 = (val | I2S_RCR2_SYNC(0U));
    val                       = 0;

    handle->config->sai->RCR2 |= I2S_RCR2_BCP(handle->config->micClockPolarity);

    val = 0;
    val = (handle->config->sai->RCR4 & (~I2S_RCR4_SYWD_MASK));
    val |= I2S_RCR4_SYWD(kSAI_WordWidth32bits - 1U);
    handle->config->sai->RCR4 = val;

    handle->config->sai->RCR2 &= ~I2S_RCR2_DIV_MASK;
    handle->config->sai->RCR2 |= I2S_RCR2_DIV((mclkSourceClockHz / bclk) / 2U - 1U);

#if USE_NEW_PDM_PCM_LIB
    /* USE_NEW_PDM_PCM_LIB expects PDM data in MSB First format which requires First Bit Shifted to be set as follows:
     * FBT value must be greater than or equal to the word width when configured for MSB First. */
    handle->config->sai->RCR5 = I2S_RCR5_WNW(31U) | I2S_RCR5_W0W(31U) | I2S_RCR5_FBT(31U);
#else
    handle->config->sai->RCR5 = I2S_RCR5_WNW(31U) | I2S_RCR5_W0W(31U) | I2S_RCR5_FBT(0U);
#endif /* USE_NEW_PDM_PCM_LIB */

    handle->config->sai->RMR = 0U;

    handle->config->sai->RCR3 &= ~I2S_RCR3_RCE_MASK;
    handle->config->sai->RCR3 |= I2S_RCR3_RCE(handle->config->saiChannelMask);

    handle->config->sai->RCR1 = FSL_FEATURE_SAI_FIFO_COUNT / 2U;

    handle->dma->EEI |= (1 << handle->dmaChannel);
    handle->dma->SERQ = DMA_SERQ_SERQ(handle->dmaChannel);

    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | kSAI_FIFORequestDMAEnable);

    return kPdmMicSuccess;
}

void PDM_MIC_StartMic(sln_mic_handle_t *handle)
{
    s_skipDirtyFrames = SKIP_DIRTY_FRAMES;

    if (handle->config->sai == SAI1)
    {
        CLOCK_EnableClock(kCLOCK_Sai1);
    }
    else if (handle->config->sai == SAI2)
    {
        CLOCK_EnableClock(kCLOCK_Sai2);
    }
    else if (handle->config->sai == SAI3)
    {
        CLOCK_EnableClock(kCLOCK_Sai3);
    }

    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | I2S_RCSR_RE_MASK);
    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | kSAI_FIFOErrorFlag);

    /* Wait for the dirty frames to be skipped so the mics are fully running. */
    while (s_skipDirtyFrames)
    {
        vTaskDelay(1);
    }
}

void PDM_MIC_StopMic(sln_mic_handle_t *handle)
{
    NVIC_DisableIRQ(handle->dmaIrqNum);
    DMAMUX_DisableChannel(DMAMUX, handle->dmaChannel);

    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) & ~(I2S_RCSR_RE_MASK | I2S_RCSR_BCE_MASK));
    /* RE(Receiver Enable) bit and BCE(Bit Clock Enable) bit will be actually written to the RCSR register
     * at the end of the current frame. We should wait for the write to be done, because the following
     * reads of this register could use the old values.
     */
    while ((handle->config->sai->RCSR & (I2S_RCSR_RE_MASK | I2S_RCSR_BCE_MASK)) != 0)
        ;

    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | kSAI_FIFOErrorFlag | I2S_RCSR_FR_MASK);

    if (handle->config->sai == SAI1)
    {
        CLOCK_DisableClock(kCLOCK_Sai1);
    }
    else if (handle->config->sai == SAI2)
    {
        CLOCK_DisableClock(kCLOCK_Sai2);
    }
    else if (handle->config->sai == SAI3)
    {
        CLOCK_DisableClock(kCLOCK_Sai3);
    }
}

#endif /* _PDM_TO_PCM_TASK_H_ */
