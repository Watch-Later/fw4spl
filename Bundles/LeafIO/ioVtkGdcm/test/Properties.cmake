
set( NAME ioVtkGdcmTest )
set( VERSION  )
set( TYPE TEST )
set( DEPENDENCIES fwTest vtkGdcmIO )
set( REQUIREMENTS io ioVtkGdcm )

set(CPPUNITTEST_OPTIONS BUNDLE ioVtkGdcm WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
