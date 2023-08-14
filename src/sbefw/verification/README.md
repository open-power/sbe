### Input-output correlations for scom filtering and invalid address check feature

| **_Sl<br>No_** | **_Input via<br>Secure Header<br>Status_** | **_Input via<br>Secure Header<br>_** | **_Output via<br>LFR Bit19_** |             **_Input via<br>ATTR_BOOT_FLAGS_**            |          **_Input via<br>ATTR_BOOT_FLAGS_**         |       **_Output_**       |             **_Output_**             |
|:--------------:|:------------------------------------------:|:------------------------------------:|:-----------------------------:|:---------------------------------------------------------:|:---------------------------------------------------:|:------------------------:|:------------------------------------:|
|                |   **_Secure Header<br>Pass or <br>Fail_**  |  **_Production<br>or<br>Imprint _**  |     **_Secure <br>Mode_**     | **_Disable Invalid<br>Address check<br>(Scratch3-bit9)_** | **_Disable SCOM <br>Filtering<br>(Scratch3-bit8)_** | **_Scom <br>Filtering_** | **_Invalid <br>Address  <br>Check_** |
|     **_1_**    |                    Fail                    |                   x                  |            _Enable_           |                             x                             |                          x                          |         _Enabled_        |               _Enabled_              |
|     **_2_**    |                    Pass                    |                Imprint               |           _Disable_           |                             1                             |                          1                          |        _Disabled_        |              _Disabled_              |
|     **_3_**    |                    Pass                    |              Production              |            _Enable_           |                             x                             |                          x                          |         _Enabled_        |               _Enabled_              |

### Input-output correlations for SAB

| **_Sl<br>No_** |  **_Input via<br>Jumper/SMD <br>Value_** | **_Input via<br>LFR Bit19_** |   **_Input via<br>ATTR_SECURITY_MODE<br>_**   |       **_Output <br>via<br>SAB_**      |
|:--------------:|:----------------------------------------:|------------------------------|:---------------------------------------------:|:--------------------------------------:|
|                | **_Security<br>Disable<br>(2801-bit5)_** | **_Secure <br> Mode_**       | **_Disable <br>Security<br>(Scratch3-bit6)_** | **_Secure Access Bit<br>(2801-bit4)_** |
|     **_1_**    |         1<br>(Requested Disable)         |              _x_             |                       x                       |            _x<br>Non Secure_           |
|     **_2_**    |                     0                    |           _Enable_           |                       x                       |                   _1_                  |
|     **_3_**    |                     0                    |           _Disable_          |                       1                       |                   _0_                  |
| **_4_**        |                     0                    |           _Disable_          |                       0                       |                   _1_                  |