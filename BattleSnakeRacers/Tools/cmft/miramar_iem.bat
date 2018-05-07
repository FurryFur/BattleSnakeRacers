cmft   %* --inputFacePosX miramar_rt.tga     ^
          --inputFaceNegX miramar_lf.tga     ^
          --inputFacePosY miramar_up.tga     ^
          --inputFaceNegY miramar_dn.tga     ^
          --inputFacePosZ miramar_bk.tga     ^
          --inputFaceNegZ miramar_ft.tga     ^
          ::Filter options                   ^
          --filter irradiance                ^
          --srcFaceSize 0                    ^
          --dstFaceSize 256                  ^
          --lightingModel blinnbrdf          ^
          --edgeFixup none                   ^
          ::Processing devices               ^
          --numCpuProcessingThreads 4        ^
          --useOpenCL true                   ^
          --clVendor nvidia                  ^
          --deviceIndex 0                    ^
          --deviceType gpu                   ^
          ::Aditional operations             ^
          --inputGammaNumerator 2.2          ^
          --inputGammaDenominator 1.0        ^
          --outputGammaNumerator 1.0         ^
          --outputGammaDenominator 2.2       ^
          ::Output                           ^
          --outputNum 1                      ^
          --output0 "miramar_iem"            ^
          --output0params dds,bgra8,cubemap
