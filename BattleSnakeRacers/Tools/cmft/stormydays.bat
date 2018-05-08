cmft   %* --inputFacePosX stormydays_rt.tga  ^
          --inputFaceNegX stormydays_lf.tga  ^
          --inputFacePosY stormydays_up.tga  ^
          --inputFaceNegY stormydays_dn.tga  ^
          --inputFacePosZ stormydays_bk.tga  ^
          --inputFaceNegZ stormydays_ft.tga  ^
          ::Filter options                   ^
          --filter none                      ^
          --srcFaceSize 0                    ^
          --dstFaceSize 0                    ^
          ::Output                           ^
          --outputNum 1                      ^
          --output0 "stormydays"             ^
          --output0params dds,bgra8,cubemap
