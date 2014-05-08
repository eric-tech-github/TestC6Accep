/*==================================================================*/
/*  Copyright (c) 2010, Texas Instruments Incorporated              */
/*  All rights reserved.                                            */
/*                                                                  */
/*                                                                  */
/* ======== appMain.c ========                                      */
/*This is main app file that sets the testing mechnaism and calls   */
/*  functions to test kernels in C6Accel in C6ACCEL                 */
/*                                                                  */
/*                                                                  */
/*  Version: 0.0.1                                                  */
/*==================================================================*/

/* Standard includes */
#include <stdio.h>
#include <math.h>

/* Codec Engine and xdc includes */ 
#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/CERuntime.h>
#include <ti/sdo/ce/osal/Memory.h>

/* system time include*/
#include <sys/time.h>

/* Test application include */
#include "appMain.h"

/* Include C6ACCEL headers */
#include "../c6accelw/c6accelw.h"
#include "c6accel_testfxns.h"

/* Define Engine ,algorithm and application names */

/* Pick engine name based on platform */
#if (PLATFORM == 3530)
 #define ENGINENAME "omap3530"
 #warning ENGINENAME "omap3530"
#elif (PLATFORM == 138)
 #define ENGINENAME "omapl138" 
 #warning ENGINENAME "omapl138"
 #define DEVICE_FLOAT  
#endif
#define ALGNAME "c6accel"
#define APPNAME "app"

/* Memory buffers used for test */
Int8 *pSrcBuf_16bpp=NULL;
Int8 *pRefBuf_16bpp=NULL;
Int8 *pOutBuf_16bpp=NULL;
Int8 *pWorkingBuf_16bpp=NULL;
Int8 *pWorkingBuf2_16bpp=NULL;
#ifdef DEVICE_FLOAT
Int8 *pWorkingBuf3_16bpp=NULL;
#endif

/* Following vars/code are required by MACROs defined in benchmark.h*/
FILE* benchmarkFd;

/* Codec Engine engine and codec labels, defined in cfg file: */
static String algName      = ALGNAME;
static String engineName   = ENGINENAME;
static String progName     = APPNAME;

/* This object is used in MACRO in benchmark.h */
#include "timeObj.h"
Time_Object sTime;
UInt32      time;

void Time_reset(Time_Object *sTime)
{
 struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    sTime->original = tv;
    sTime->previous = tv;

    return;
}

void Time_delta(Time_Object* sTime, UInt32 *deltaPtr)
{
    struct timeval tv;
    time_t         s;
    suseconds_t    us;

    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    s = tv.tv_sec - sTime->previous.tv_sec;
    us = tv.tv_usec - sTime->previous.tv_usec;

    *deltaPtr = s * 1000000l + us;

    sTime->previous = tv;

    return;
}

/* Now include benchmark.h after it's supporting functions/vars are declared*/
#include "benchmark.h"

extern void CERuntime_init(void);

/*Define a C6Accel Handle to call the abstracted wrapper APIs*/
C6accel_Handle hC6 = NULL;

/******************************************************************************
 * appMain
 ******************************************************************************/
Int main(Int argc, Char *argv[])
{
    UInt32 framesize;
    Memory_AllocParams memParams = Memory_DEFAULTPARAMS;
    int i;
    
    printf("******************************************************************************\n");
    printf("Sample application for testing kernels in C6Accel started.\n");
    printf("******************************************************************************\n");
    /* This call must be made before the Memory_xxx() functions as it is required for the tracing functions
     in all the codec engine APIs that are used*/
    CERuntime_init();

    /* Reset timeObj used for benchmarking*/
    Time_reset(&sTime);

    /* Create call generates a C6ACCEL handle */
    hC6 = C6accel_create(engineName, NULL,algName, NULL);

    /*Check for failure*/
    if ( hC6 == NULL)
       {printf("%s: C6accel_create() failed \n",progName);
        goto end;
    }
   
    /* Create buffers for use by algorithms */

    /* Want to use cached & contiguous memory to get best performance from cortex when it also uses the buffers.*/
    memParams.flags = Memory_CACHED;
    memParams.type = Memory_CONTIGHEAP;

    /* Size all buffers for 6 bytes, to cope with worst case 16 bit 422Planar*/
    framesize = (MAX_WIDTH * MAX_HEIGHT * sizeof(Int32)*3/2);

    /* Create 16bit buffers for use by algorithms*/
    pSrcBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pSrcBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pSrcBuf_16bpp, framesize);
    }
    
    pOutBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pOutBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pOutBuf_16bpp, framesize);
    }
    
    pRefBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pRefBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pRefBuf_16bpp, framesize);
    }

    pWorkingBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pWorkingBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pWorkingBuf_16bpp, framesize);
    }
    
    pWorkingBuf2_16bpp = Memory_alloc(framesize, &memParams);
    if (pWorkingBuf2_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pWorkingBuf2_16bpp, framesize);
    }
  
    #ifdef DEVICE_FLOAT
    pWorkingBuf3_16bpp = Memory_alloc(framesize, &memParams);
    if (pWorkingBuf3_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pWorkingBuf3_16bpp, framesize);
    }
    #endif
    
    /* Set C6Accel instance for asynchronous call*/
    C6Accel_setSync(hC6);
    if (C6Accel_readCallType(hC6) == SYNC){
       printf("->  Synchronous Calls\n");
     }
     else 
       printf("->  Asynchronous Calls\n");

    /* open file for reporting benchmarking output*/
    OPEN_LOG_FILE("benchmarking.txt");
   
    /* Call test functions for kernels in C6accel*/
    LOG_STRING("IMGLib Functions\n");
    LOG_STRING("640x480 8bit/pixel b/w Test Image \n");
    
    printf("-----------------------------------------------------------------------------\n");
    printf("Test for Image processing functions in C6Accel: \n");
    printf("-----------------------------------------------------------------------------\n");
    /* Test functions to test Imaging kernels in C6Accel */
    c6accel_test_IMG_muls(hC6,WIDTH,HEIGHT);
   

    /* Implementation of this function limits
    the rows and columns of matrices to be multiples of 4 and r1 >8 */
    c6accel_test_DSP_MATMUL(hC6,ROW1,COL1,COL2,SHIFT);

    /* Set C6Accel instance for asynchronous call*/
    C6Accel_setSync(hC6);
     if (C6Accel_readCallType(hC6) == SYNC){
       printf("-> Synchronous Calls\n");
     }
     else 
       printf("-> Asynchronous Calls\n");

   CLOSE_LOG_FILE();

end:
     // Tear down C6ACCEL
    if (hC6)
       C6accel_delete(hC6);

    if(pSrcBuf_16bpp)
        Memory_free(pSrcBuf_16bpp, framesize, &memParams);

    if(pOutBuf_16bpp)
        Memory_free(pOutBuf_16bpp, framesize, &memParams);

    if(pRefBuf_16bpp)
        Memory_free(pRefBuf_16bpp, framesize, &memParams);

    if(pWorkingBuf_16bpp)
        Memory_free(pWorkingBuf_16bpp, framesize, &memParams);

    if(pWorkingBuf2_16bpp)
        Memory_free(pWorkingBuf2_16bpp, framesize, &memParams);

    #ifdef DEVICE_FLOAT
    if(pWorkingBuf3_16bpp)
        Memory_free(pWorkingBuf3_16bpp, framesize, &memParams);
    #endif

    printf("******************************************************************************\n");
    printf("All tests done.\n");
    printf("******************************************************************************\n");
    printf("\n");

   return (0);
}



/*Functions to compare arrays for the test, used across test files*/
unsigned int compare8bitArrays(unsigned char *pSrc1, unsigned char *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++){
           if (pSrc1[i] != pSrc2[i]){  
               printf("Pixel mismatch at offset %i, C6Accel = %x Native = %x\n",
                      i,pSrc1[i],pSrc2[i]);
               return(0);
            }
      }
     return (1);

}


unsigned int compare16bitArrays(unsigned short *pSrc1, unsigned short *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++){
            if (pSrc1[i] != pSrc2[i]){
                printf("Pixel mismatch at offset %i, C6Accel = %x Native = %x\n",i,pSrc1[i],pSrc2[i]);
               return(0);
               }
          }
     return (1);
}



unsigned int compare32bitArrays(unsigned int *pSrc1, unsigned int *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++){
           if (pSrc1[i] != pSrc2[i]){
              printf("Pixel mismatch at offset %i, C6Accel = %x Native = %x\n",i,pSrc1[i],pSrc2[i]);
               return(0);
            }
       }
     return (1);
}


unsigned int comparefloatArrays(float *pSrc1, float *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++){
           // For float do comparison by being closer than a small error  
           if (abs(pSrc1[i]-pSrc2[i])> 0.00001){
              printf("Pixel mismatch at offset %i, C6Accel = %f Native = %f\n",i,pSrc1[i],pSrc2[i]);
               return(0);
           }
        }
     return (1);
}

unsigned int comparedoubleArrays(double *pSrc1, double *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++){
           // For float do comparison by being closer than a small error  
           if (abs(pSrc1[i]-pSrc2[i])> 0.00001){
              printf("Pixel mismatch at offset %i, C6Accel = %f Native = %f\n", i,pSrc1[i],pSrc2[i]);
               return(0);
           }
       }
     return (1);
}

/*=============================================================================*/
/*                  End of File appMain.c                                      */
/*                              Version : 0.0.1                                */
/*=============================================================================*/

