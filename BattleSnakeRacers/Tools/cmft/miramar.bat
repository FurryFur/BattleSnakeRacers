cmft   %* --inputFacePosX miramar_rt.tga     ^
          --inputFaceNegX miramar_lf.tga     ^
          --inputFacePosY miramar_up.tga     ^
          --inputFaceNegY miramar_dn.tga     ^
          --inputFacePosZ miramar_bk.tga     ^
          --inputFaceNegZ miramar_ft.tga     ^
          ::Filter options                   ^
          --filter none                      ^
          --srcFaceSize 0                    ^
          --dstFaceSize 0                    ^
          ::Output                           ^
          --outputNum 1                      ^
          --output0 "miramar"                ^
          --output0params dds,bgra8,cubemap
