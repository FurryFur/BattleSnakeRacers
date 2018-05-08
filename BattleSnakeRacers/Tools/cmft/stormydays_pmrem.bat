cmft   %* --inputFacePosX stormydays_rt.tga  ^
          --inputFaceNegX stormydays_lf.tga  ^
          --inputFacePosY stormydays_up.tga  ^
          --inputFaceNegY stormydays_dn.tga  ^
          --inputFacePosZ stormydays_bk.tga  ^
          --inputFaceNegZ stormydays_ft.tga  ^
          ::Filter options                   ^
          --filter radiance                  ^
          --srcFaceSize 0                    ^
          --excludeBase false                ^
          --mipCount 9                       ^
          --glossScale 10                    ^
          --glossBias 1                      ^
          --lightingModel blinnbrdf          ^
          --edgeFixup none                   ^
          --dstFaceSize 256                  ^
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
          --generateMipChain true            ^
          ::Output                           ^
          --outputNum 1                      ^
          --output0 "stormydays_pmrem"       ^
          --output0params dds,bgra8,cubemap
