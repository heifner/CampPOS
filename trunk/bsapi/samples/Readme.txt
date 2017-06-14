=============
BSAPI Samples
=============

This document lists samples included in BSAPI SDK. Below see the brief
description of samples from the most basic one to more advanced ones.

Note that not all samples are buildable on every platform. Still they are 
distributed as we think their source code still provides a valuable 
information.

----------
FirstStep
----------
This is very straithforward sample, intended to introduce the basics of BSAPI.

The code is platform independent. Besides BSAPI, it uses only the standard C 
library.

   Highlights:
    -- initialization: ABSInitialize(), ABSTerminate()
    -- session management: ABSOpen(), ABSClose()
    -- basic biometry: ABSEnroll(), ABSVerify()
    -- callback

---------
Biometry
---------
This sample is a more complex variant of the FirstStep sample. It resembles a 
typical real-world application. It manages enrolled templates in memory and 
provides several basic functions manipulating with the template set.

Its user interface is textual. Besides BSAPI, it uses only the standard C 
library.
   
   Highlights:
    -- session management: ABSOpen(), ABSClose()
    -- most of BSAPI biometric functions

-----------
Navigation
-----------
The AuthenTec fingerprint sensor can be used as a pointing device, like a mouse or 
a touchpad. This example demonstrates this feature by moving an image. The 
example heavily uses Win32API.

   Highlights:
    -- BSAPI & multihtreading
    -- navigation: ABSNavigate()
    -- interactive operation cancellation: ABSCancelOperation()

-----
Grab
-----
This sample is a demonstration of grab and raw grab capabilities. The grab
provides a way how to easily retrieve fingerprint images from the fingerprint 
device. The raw grab is a more low level operation and it is intended for 
applications which prefer to use their own or 3rd party biometric algorithms.

This sample uses Win32API.
   
   Highlights:
    -- grab: ABSGrab(), ABSRawGrab()
    -- newer grab functions: ABSListImageFormats(), ABSGrabImage() and
            ABSRawGrabImage()
    -- binarization of images: ABSBinarizeSampleImage()

-----
LiveImagery
-----
This sample shows repeated grab on area sensor. In order to achieve the highest
possible frame rate, the sample disables waiting for an acceptable finger image
and image post-processing (ABS_PKEY_WAIT_FOR_ACCEPTABLE key), uses 508dpi image
format (not applicable fot TCD21 device - do not specify ABS_PKEY_IMAGE_FORMAT 
for TCD21), and listens to low-level callbacks (ABS_OPERATION_FLAG_LL_CALLBACK) to
avoid artificial delays between subsequent messages. If device allows
authentification without an encryption of a communication, the frame rate can be 
roughly doubled for a penalty of low security (ABS_PARAM_SENSOR_SECURITY). With
a standard device ABS_SENSOR_SECURITY_LOW setting has no impact.

This sample uses Win32API.
   
   Highlights:
    -- Raw grab settings for the highest possible frame rate
    -- Low-level callbacks

------------
AnsiIsoImage
------------
This sample shows how to grab an image, collect necessary information and convert
the image to ANSI/ISO format. ABSGrabImage is used for grabbing, additional
information is obtained from ABS_IMAGE_FORMAT structure of format used
for image grab and from device properties received via ABSGetDeviceProperty call.

Its user interface is textual. Besides BSAPI, it uses only the standard C 
library.

   Highlights:
    -- grab: ABSGrabImage()
    -- additional information source: ABSListImageFormats(), 
            ABSGetDeviceProperty()

------------
BSAPIFnPreview
------------
BSAPI Function Preview tool. Present only in Windows version. Implements almost all BSAPI functions in a graphical user interface where each function has its button and most functions have also controls to set their parameters. This tool can be used for simulating target application function flow with testing different parameters. There is no special user guide for this tool - studying the main BASPI.pdf document will provide answers on how to use this tool.
Visual Basic 6 sources are not available in the Lite version.
