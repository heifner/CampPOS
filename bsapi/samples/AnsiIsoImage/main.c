/* 2009 (C) Upek, inc.
 * BSAPI sample AnsiIsoImage
 * 
 * This sample shows how to grab image, convert it to ANSI/ISO format
 * and store to a file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if defined __APPLE__  &&  defined __MACH__
    #include <BSApi/bsapi.h>  /* Mac OS X */
#else
    #include <bsapi.h>  /* other OSes */
#endif


/* Define one and only one of the following two symbols 
 * to choose output format */

#define OUTPUT_FORMAT_ANSI
/* #define OUTPUT_FORMAT_ISO */

/* Check if output output format is selected correctly */

#if (((!defined(OUTPUT_FORMAT_ANSI)) && (!defined(OUTPUT_FORMAT_ISO))) || (defined(OUTPUT_FORMAT_ANSI) && defined(OUTPUT_FORMAT_ISO)))
#error "Select one and only one of output formats!"
#endif
 
/* Used to supress compiler warnings */
#ifndef UNREFERENCED_PARAMETER
    #define UNREFERENCED_PARAMETER(param)   (void)param
#endif
 
/* Writes info about last error.
 */
static void
status_info(ABS_STATUS status)
{
    ABS_DWORD code;
    const ABS_CHAR* message;
    
    if(status == ABS_STATUS_OK)
        return;
    
    /* ABSGetLastErrorInfo() provides some diagnostical informations
     * about the last BSAPI error which occured in the current thread.
     *
     * Please note that in real applications these informations are 
     * not intended for end user. 
     */
    ABSGetLastErrorInfo(&code, &message);
    printf("   status: %ld\n", (long)status);
    printf("   code:   %ld\n", (long)code);
    printf("   message: '%s'\n", message);
}
 
/* Callback is the primary way how the interactive biometric operations 
 * can interract with the user. Each BSAPI function which expects user 
 * to interact with the fingerprint sensor, takes pointer to structure
 * ABS_OPERATION as a parameter. One of its members is pointer to a 
 * function.
 *
 * BSAPI uses the function as a communication channel between the biometry
 * logic and user. 
 *
 * The callback is the way how to bind the biometry to GUI in your 
 * applications.
 */
static void BSAPI
callback(const ABS_OPERATION* p_operation, ABS_DWORD msg, void* data)
{
    UNREFERENCED_PARAMETER(p_operation);
    
    switch(msg) {
        /* These messages just inform us how the interactive operation
         * progresses. Typical applications do not need it. */
        case ABS_MSG_PROCESS_BEGIN:
        case ABS_MSG_PROCESS_END:
            break;
            
        /* On some platforms, the biometric operastion can be suspended
         * when other process acquires sensor for other operation. */
        case ABS_MSG_PROCESS_SUSPEND:
            printf("   operation has been suspended\n");
            break;
        case ABS_MSG_PROCESS_RESUME:
            printf("   operation has been resumed\n");
            break;
            
        /* Sometimes some info how the operation progresses is sent. */
        case ABS_MSG_PROCESS_PROGRESS:
        {
            ABS_PROCESS_PROGRESS_DATA* progress_data = 
                                    (ABS_PROCESS_PROGRESS_DATA*) data;
            if(progress_data->Percentage <= 100) {
                printf("   operation in progress (%d%%)...\n", 
                                            (int)progress_data->Percentage);
            } else {
                printf("   operation in progress...\n");
            }
            break;
        }
        case ABS_MSG_PROCESS_SUCCESS:
            printf("   success\n");
            break;
        case ABS_MSG_PROCESS_FAILURE:
            printf("   failure\n");
            break;
        
        /* Prompt messages should inform the user that he should do 
         * something. */
        case ABS_MSG_PROMPT_SCAN:
            printf("   swipe the finger\n"); 
            break;
        case ABS_MSG_PROMPT_TOUCH:
            printf("   touch the sensor\n");
            break;
        case ABS_MSG_PROMPT_KEEP:
            printf("   keep finger on the sensor\n"); 
            break;
        case ABS_MSG_PROMPT_LIFT:
            printf("   lift your finger away from the sensor\n");
            break;
        case ABS_MSG_PROMPT_CLEAN:
            printf("   clean the sensor\n"); 
            break;
        
        /* Quality messages come if something went wrong. E.g. the user
         * did not scan his finger in the right way. */
        case ABS_MSG_QUALITY_CENTER_HARDER:
            printf("   bad quality: center and harder\n"); 
            break;
        case ABS_MSG_QUALITY_CENTER:
            printf("   bad quality: center\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_LEFT:
            printf("   bad quality: too left\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_RIGHT:
            printf("   bad quality: too right\n"); 
            break;
        case ABS_MSG_QUALITY_HARDER:
            printf("   bad quality: harder\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_LIGHT:
            printf("   bad quality: too light\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_DRY:
            printf("   bad quality: too dry\n");
            break;
        case ABS_MSG_QUALITY_TOO_SMALL:
            printf("   bad quality: too small\n");
            break;
        case ABS_MSG_QUALITY_TOO_SHORT:
            printf("   bad quality: too short\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_HIGH:
            printf("   bad quality: too high\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_LOW:
            printf("   bad quality: too low\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_FAST:
            printf("   bad quality: too fast\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_SKEWED:
            printf("   bad quality: too skewed\n"); 
            break;
        case ABS_MSG_QUALITY_TOO_DARK:
            printf("   bad quality: too dark\n"); 
            break;
        case ABS_MSG_QUALITY_BACKWARD:
            printf("   bad quality: backward movement detected\n"); 
            break;
        case ABS_MSG_QUALITY_JOINT:
            printf("   bad quality: joint detected\n"); 
            break;
        
        /* Navigation messages are sent only from ABSNavigate. Its not used
         * in this sample but we list the messages here for completeness. */
        case ABS_MSG_NAVIGATE_CHANGE:
        case ABS_MSG_NAVIGATE_CLICK:
            break;
            
        /* Real application would probably use some GUI to provide feedback
         * for user. On these messages the GUI dialog should be made vsiible
         * and invisible respectivelly. */
        case ABS_MSG_DLG_SHOW:
        case ABS_MSG_DLG_HIDE:
            break;
            
        /* Idle message can come only if flag ABS_OPERATION_FLAG_USE_IDLE
         * was specified in ABS_OPERATION::dwFlags (i.e. never in this sample).
         * If the flag is specified, this message comes very often, hence 
         * giving the callback a chance to cancel the operation with 
         * ABSCancelOperation() without long time delays. In multithreaded 
         * applications, canceling the operation from another thread can be
         * better alternative. Consult BSAPI documentation for more info about
         * the topic. */
        case ABS_MSG_IDLE:
            break;
    }
}

/* BSAPI session handle. */
static ABS_CONNECTION gConn = 0;

/* Pointer to ABS_OPERATION is taken as a parameter by those BSAPI 
 * funtions which work as interactive operation. The main purpose of it is 
 * to pass pointer to the callback function into the interactive functions. 
 *
 * It also allows to specifie some flags and/or timeout.
 *
 * In this sample we reuse this one operation instance. In real complex 
 * application you may need to use special ABS_OPERATION instance for 
 * each BSAPI interactive operation. This allows you to use specialized
 * callback for them, past various flags etc.
 */
static ABS_OPERATION op = { 
    /* ID of the operation. We don't need to identify the operation in this 
     * sample. When non-zero, the ID identifies the operation and allows it
     * to be canceled from any other thread with ABSCancelOperation(). */
    0,         
    
    /* Arbitrary pointer, which allows application to pass any data into
     * the callback. Not used in this sample. */
    NULL,      
    
    /* Pointer to a simple callback implementation function. */
    callback,  
    
    /* Timeout. For example, lets set timeout to 60 sec. Note the value does 
     * not limit how long the operation (e.g. ABSVerify()) can take. The 
     * timeout only specifies time the operation waits for user to put his 
     * finger on a sensor. Zero would mean no timeout (i.e. the operation can 
     * never end if user never puts his finger onm the sensor.) */
    60000,
    
    /* By default BSAPI places short time delays between sending some important
     * callback messages. The purpose of this is to guarantee that if multiple
     * messages come very closely in sequence, then the user still has enough
     * time to see all the messages and not just the lat one of the fast
     * sequence.
     *
     * For application developer, this simplifies callback implementation
     * which in most cases can be just showing an appropriate message in a 
     * window or dialog.
     *
     * However the time delays are not needed when user can see all history
     * of the messages, e.g. (as in this sample) the messages are outputted
     * to standard output stream. Hence we disable the time delays with with 
     * the flag ABS_OPERATION_FLAG_LL_CALLBACK here. */
    ABS_OPERATION_FLAG_LL_CALLBACK
};

/* Only some devices support ABSGrabImage() and ABSRawGrabImage(). */
static ABS_BOOL gGrabImageSupported = ABS_FALSE;

/* During initialization, we choose a format we will then use */
static ABS_IMAGE_FORMAT gImageFormat;

/* This function detects whether the device supports ABSGrabImage and 
 * ABSRawGrabImage() functions. If yes, it also setups the global variable
 * imageFormat, used as parameter for those two functions. */
static void
SetupImageFormat(void)
{
    ABS_STATUS status;
    ABS_DWORD dwCount;
    ABS_IMAGE_FORMAT* pImageFormatList;
    
    status = ABSListImageFormats(gConn, &dwCount, &pImageFormatList, 0);
    if(status == ABS_STATUS_OK) {
        if(dwCount > 0) {
            gGrabImageSupported = ABS_TRUE;
            
            /* We will simply use the first image format in the list. Real 
             * application could allow user to choose which one to use. */
            gImageFormat = pImageFormatList[0];
            
            ABSFree(pImageFormatList);
        } else {
            printf("No apprpriate image format for ABSGrabImage found" );
            gGrabImageSupported = ABS_FALSE;
        }
    } else {
        if(status == ABS_STATUS_NOT_SUPPORTED) {
            printf("The fingerprint reader does not support ABSGrabImage function");
        } else {
            printf("ABSListImageFormats() failed.");
            status_info(status);
        }        
        gGrabImageSupported = ABS_FALSE;
    }
}

/* Helper function for writing unsigned values in big endian format */

void PutUintBigEndian( ABS_BYTE **ppData, ABS_DWORD value, ABS_DWORD cntBytes )
{
    ABS_DWORD mask;
    ABS_DWORD shift;

    mask = 0xff << ((cntBytes - 1) * 8);
    shift = (cntBytes - 1) * 8;

    for (;;)
    {
        /* Special treatment for last step */

        if ( shift == 0 )
        {
            **ppData = (ABS_BYTE) (value & 0xff);
            (*ppData)++;
            break;
        }

        /* Mask value, shift down, store and advance pointer */

        **ppData = (ABS_BYTE) ((value & mask) >> shift);
        (*ppData)++;

        /* Update shift and mas values */

        shift -= 8;
        mask >>= 8;
    }
}

/* Constant data and defines needed for ANSI/ISO format */

ABS_BYTE gSignatureImageAnsiIso[] = { 'F', 'I', 'R', 0 };
#define ANSI_ISO_SPECIFICATION_IMAGE_VERSION  0x30313000
#define CBEFF_ID_UPEK  0x12


int AbsSampleToAnsiIso( const ABS_IMAGE_FORMAT *pAbsFormat, const ABS_SAMPLE_IMAGE *pImage, ABS_DWORD sensorType,
    ABS_BYTE *pOutputImage, ABS_DWORD *pLenOutput )
{
    int lenOutput = 0;
    ABS_BYTE *pData = pOutputImage;

    /* Check output buffer size first */

#ifdef OUTPUT_FORMAT_ANSI

    lenOutput = pImage->Height * pImage->Width + 36 + 14;

#endif

#ifdef OUTPUT_FORMAT_ISO

    lenOutput = pImage->Height * pImage->Width + 32 + 14;

#endif

    if(lenOutput < *pOutputImage) {
        /* Output buffer too small, return required size */
        *pLenOutput = lenOutput;
        return -1;
    }

    /* Output buffer size is good, fill real length of output data */
    *pLenOutput = lenOutput;

    /* Start with signature */
    memcpy( pData, gSignatureImageAnsiIso, sizeof(gSignatureImageAnsiIso));
    pData += sizeof(gSignatureImageAnsiIso);

    /* Specification version */
    PutUintBigEndian( &pData, ANSI_ISO_SPECIFICATION_IMAGE_VERSION, 4 );

    /* Record length - upper 2 bytes always zero */
    PutUintBigEndian( &pData, 0, 2 );

    /* Real value of the length (utilise the fact *pLenOutput is already set) */
    PutUintBigEndian( &pData, *pLenOutput, 4 );

#ifdef OUTPUT_FORMAT_ANSI

    /* CBEFF PID */
    PutUintBigEndian( &pData, CBEFF_ID_UPEK, 4 );

#endif

    /* Device ID - sensor type provided as parameter is used here */
    PutUintBigEndian( &pData, sensorType & 0xff, 2 );

#ifdef OUTPUT_FORMAT_ISO

    /* Acquisition level */
    PutUintBigEndian( &pData, 31, 2 );

#endif

    /* Number of fingers/palms */
    PutUintBigEndian( &pData, 1, 1 );

    /* Scale units (1 = ppi) */
    PutUintBigEndian( &pData, 1, 1 );

    /* Scan resolution */
    PutUintBigEndian( &pData, pAbsFormat->ScanResolutionH, 2 );
    PutUintBigEndian( &pData, pAbsFormat->ScanResolutionV, 2 );

    /* Image resolution */
    PutUintBigEndian( &pData, pImage->HorizontalDPI, 2 );
    PutUintBigEndian( &pData, pImage->VerticalDPI, 2 );

    /* Pixel depth 
     * Simplified decision is made, based of the fact we know pixel depth
     * can be only 8 or 4 here */
    PutUintBigEndian( &pData, (pImage->ColorCount < 256) ? 4 : 8, 1 );

    /* Image compression algorithm (0 = no compression) */
    PutUintBigEndian( &pData, 0, 1 );

    /* Reserved */

#ifdef OUTPUT_FORMAT_ISO

     PutUintBigEndian( &pData, 0, 2 );

#endif

#ifdef OUTPUT_FORMAT_ANSI

     PutUintBigEndian( &pData, 0, 4 );

#endif

    /* Finished with general record header, start image header */

    /* Image block length (data length + 14) */
    PutUintBigEndian( &pData, pImage->Height * pImage->Width + 14, 4 );
    
    /* Position */
    PutUintBigEndian( &pData, 0, 1 );

    /* Count of views */
    PutUintBigEndian( &pData, 1, 1 );

    /* View number */
    PutUintBigEndian( &pData, 1, 1 );

    /* Quality (254 = undefined) */
    PutUintBigEndian( &pData, 254, 1 );

    /* Impression type (0 = unknown finger) */
    PutUintBigEndian( &pData, 0, 1 );

    /* Horizontal line length */
    PutUintBigEndian( &pData, pImage->Width, 2 );

    /* Vertical line length */
    PutUintBigEndian( &pData, pImage->Height, 2 );

    /* Reserved */
    PutUintBigEndian( &pData, 0, 1 );

    /* Finished with headers, put data */
    memcpy( pData, pImage->ImageData, pImage->Height * pImage->Width );

    return 0;
}

/* Filename for output file. It is fixed for simplicity. 
 * In a real life application usually asks a user to choose filename. */

#ifdef OUTPUT_FORMAT_ANSI
static char gFilename[] = "image-ansi.bin";
#endif

#ifdef OUTPUT_FORMAT_ISO
static char gFilename[] = "image-iso.bin";
#endif

int
main(int argc, char** argv)
{
    ABS_STATUS status;
    
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
    
    printf(
        "This sample shows BSAPI as it can be used to grab image and convert it to ANSI/ISO format\n"
        "\n"
    );
    
    /* Initialize BSAPI */

    status = ABSInitialize();
    if(status != ABS_STATUS_OK) {
        printf("ABSInitilize() failed\n");
        status_info(status);
        return -1;
    }

    /* Open BSAPI session */

    status = ABSOpen("usb", &gConn);
    if(status != ABS_STATUS_OK) {
        printf("ABSOpen() failed.\n");
        status_info(status);
    }
    else
    {
        ABS_DATA *pPropData = NULL;
        ABS_DWORD sensorType = 0;
        ABS_SAMPLE_IMAGE* pImage = NULL;
        ABS_BYTE *pOutputImage = NULL;
        ABS_DWORD lenOutput = 0;

        printf("Device opened successfully.\n");
    
        /* Prepare for grab operation */
        
        SetupImageFormat();

        /* Grab image */
        
        if(gGrabImageSupported) {
            status = ABSGrabImage(gConn, &op, ABS_PURPOSE_UNDEFINED, &gImageFormat,
                &pImage, NULL, NULL, 0);
            if(status != ABS_STATUS_OK) {
                printf("ABSGrabImag() failed.");
                status_info(status);
                return -2;
            }

            /* Get sensor id */

            ABSGetDeviceProperty( gConn, ABS_DEVPROP_SENSOR_TYPE, &pPropData );
            if((status != ABS_STATUS_OK) || (pPropData->Length < sizeof(ABS_DWORD))) {
                printf("ABSGetDeviceProperty() failed.");
                status_info(status);
                return -3;
            }
            sensorType = *((ABS_DWORD *) pPropData->Data);
            ABSFree(pPropData);

            /* Convert grabbed image ANSI/ISO format */

            lenOutput = pImage->Height * pImage->Width + 36 + 14;
            pOutputImage = malloc(lenOutput);
            status = AbsSampleToAnsiIso( &gImageFormat, pImage, sensorType, pOutputImage, &lenOutput );

            if(status != 0) {
                printf("Error in image conversion\n");
            } else {

                /* Store the image to a file */

                FILE *filImage = fopen( gFilename, "wb" );
                if ( filImage != NULL ) {
                    fwrite( pOutputImage, 1, lenOutput, filImage );
                    fclose( filImage );
                } else {
                    printf("Cannot write output file \"%s\"\n", gFilename);
                }
            }
            
            /* Free grabbed image */

            ABSFree(pImage);
        }
        
        /* Close BSAPI session */

        printf("Closing the current session...\n");
    
        status = ABSClose(gConn);
        if(status != ABS_STATUS_OK) {
            printf("ABSClose() failed.\n");
            status_info(status);
        }
    }
    
    /* Free any resources allocated in ABSInitialize. */
    ABSTerminate();
    
    return 0;
}
