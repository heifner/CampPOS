/* 2008-2009 (C) Upek, inc.
 * BSAPI sample First Step
 * 
 * This sample shows very simple application using BSAPI.DLL.
 * To see more advanced sample, which resembles a real application,
 * refer to sample named Biometry.
 */


#include <stdio.h>
#include <stdlib.h>

/* To use BSAPI, we must include its main header. */
#if defined __APPLE__  &&  defined __MACH__
    #include <BSApi/bsapi.h>  /* Mac OS X */
#else
    #include <bsapi.h>  /* other OSes */
#endif


/* Macro used to supress compiler warnings */
#ifndef UNREFERENCED_PARAMETER
    #define UNREFERENCED_PARAMETER(param)   (void)param
#endif


/* For sake of readability, this sample uses very trivial error handling.
 * In case of any error, we print some info about the error and simply abort.
 */
static void
handle_error(ABS_STATUS status)
{
    ABS_DWORD code;
    const ABS_CHAR* message;
    
    if(status != ABS_STATUS_OK) {
        /* ABSGetLastErrorInfo() provides some diagnostical informations
         * about the last BSAPI error which occured in the current thread.
         *
         * Please note that in real applications these informations are 
         * not intended for end-user. 
         */
        ABSGetLastErrorInfo(&code, &message);
        
        printf("Failure: status == %ld\n", (long)status);
        printf("         code == %ld\n", (long)code);
        printf("         message == '%s'\n", message);

        exit(1);
    }
}

/* Callback is the primary way how the interactive biometric operations 
 * can interract with the user. Each BSAPI function which expects user 
 * to interact with the fingerprint sensor, takes poitner to structure
 * ABS_OPERATION as a parameter. One of its members is a pointer to a 
 * function.
 *
 * BSAPI uses the function as a communication channel between the biometry
 * logic and user. The callback is the way how to bind the biometry to user 
 * interface in your applications.
 *
 * Note that implementation of callback in thisd sample is extremely trivial,
 * to make this sample as simple as possible. 
 */
static void BSAPI
callback(const ABS_OPERATION* p_operation, ABS_DWORD msg, void* data)
{
    UNREFERENCED_PARAMETER(p_operation);
    UNREFERENCED_PARAMETER(data);
    
    switch(msg) {
        /* These messages just inform us how the interactive operation
         * progresses. Simple application as this one does not need
         * to react to them. */
        case ABS_MSG_PROCESS_BEGIN:
        case ABS_MSG_PROCESS_END:
        case ABS_MSG_PROCESS_SUSPEND:
        case ABS_MSG_PROCESS_RESUME:
        case ABS_MSG_PROCESS_PROGRESS:
        case ABS_MSG_PROCESS_SUCCESS:
        case ABS_MSG_PROCESS_FAILURE:
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
        
        /* Navigation messages are sent only from ABSNavigate. It is not used
         * in this sample but we list the messages here for completeness. */
        case ABS_MSG_NAVIGATE_CHANGE:
        case ABS_MSG_NAVIGATE_CLICK:
            break;
            
        /* Real application would probably use some popup GUI dialog to 
         * provide feedback for user. On these messages the GUI dialog should 
         * be made visible and invisible respectivelly. */
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

int
main(int argc, char** argv)
{
    /* ABS_STATUS is return code of most BSAPI functions. If the function
     * succeeds, it will be set to ABS_STATUS_OK. */
    ABS_STATUS res;
    
    /* ABS_CONNECTION represents a session with the fingerprint device. */
    ABS_CONNECTION conn;
    
    /* Pointer to ABS_OPERATION is taken as a parameter by those BSAPI 
     * funtions which work as interactive operations. In this sample we 
     * use it only to pass pointer to the callback function. */
    ABS_OPERATION op = { 0, NULL, callback, 0, 0 };
    
    /* BIR represents fingerprint template data. For needs of this sample
     * you can understand it as an opaque structure representing the finger 
     * from biometric point of view. Image of the scanned finger cannot be 
     * reconstructed from it, but it holds sufficient data in order to compare
     * two different templates whether they match or not. */
    ABS_BIR* bir;
    
    /* Helper variable used as an output parameter from ABSVerify(). 
     * See below. */ 
    ABS_LONG result;
    
    
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
    
    
    printf(
        "This is trivial BSAPI sample. It presents few basic BSAPI functions.\n"
    );
    
    
    /* Before we can use any other BSAPI function, we have to initialize 
     * the library first. */
    printf(
        "\nABSInitialize() .. First of all BSAPI must be initialized before\n"
        "any other its function may be called.\n"
    );
    res = ABSInitialize();
    handle_error(res);

    /* Then we need to establish new session with fingerprint sensor. 
     * We assume the fingerprint device is an USB device. */
    printf(
        "\nABSOpen() .. Then we have to open session with fingerprint device.\n"
    );
    res = ABSOpen("usb", &conn);
    handle_error(res);

    /* Now we enroll new finger. I.e. it asks user to scan finger several 
     * times to create new high-quality fingerprint template. Note we pass 
     * pointer to the ABS_OPERATION structure there, so that our callback is 
     * called from the function. */
    printf(
        "\nABSEnroll() .. This function asks user to scan his finger few\n"
        "times to create a fingerprint template from it. \n"
        "The template holds a biometric data describing the finger.\n"
        "Image of the finger cannot be reconstructed from the template, it\n"
        "servers only as input for biometric algorithms.\n"
    );
    res = ABSEnroll(conn, &op, &bir, 0);
    handle_error(res);
    
    /* We let the user to scan his finger once again and compare his finger
     * with the template we acquired above. Again we pass in the pointer to 
     * ABS_OPERATION. */
    printf(
        "\nABSVerify() .. Now we ask user to scan again to retrieve his\n"
        "template. The function compares the new template with the template\n"
        "from the previous enrollment.\n"
    );
    res = ABSVerify(conn, &op, 1, &bir, &result, 0);
    handle_error(res);
    if(result == -1)
        printf("The templates do NOT match.\n");
    else
        printf("The templates do match.\n");
    
    /* We do not need the enrolled template anymore, so release its memory. */
    printf(
        "\nABSFree() .. After we do not need it it, we release the enrollment\n"
        "template from memory.\n"
    );
    ABSFree(bir);
    
    /* Close the session */
    printf(
        "\nABSClose() .. When not needed anymore, we close the session.\n"
        "with the fingerprint device.\n"
    );
    ABSClose(conn);
    
    /* Free all resources allocated in ABSInitialize() */
    printf(
        "\nABSTerminate() .. The initial call to ABSInitialize() should be\n"
        "paired with corresponding call to ABSTerminate().\n"
    );
    ABSTerminate();

    /* Finally, we are done. */
    printf("\nBye bye...\n");    
    return 0;
}
