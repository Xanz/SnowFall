# Console Commands
This is the a list of all the games console commands. I have added the previous command name just in case people are coming from different Id Tech engines so you can search for the equivalent command. I will also capture previous command names in case they change between engine versions.

| Console command | Description | Previous command name |
| --- | --- | --- |
| volume | Adjusts the game volume. It scales between 0 - 1. Replaces the old version that used to scale based on dB. | s_volume_dB |
| sensitivity | Controls the game sensitivity. |  |
| m_rawinput | Value between 0 - 1. 1 turns on raw mouse input. If you are used to playing competitive shooters you should probably set this to 1. |  |
| r_aspectRatio | The possible values are 0, 1, 2: 0 is 4:3, 1 is 16:9, 2 is 16:10. This is not tied to resolution. (TODO: disable this command and set this manually in the game code.) |  |
| r_multiSamples | Sets the renderers multi samples value. This is essentially just MSAA, the values accepted are 0 to 8. I would recommend setting this to 8 as the game looks better and there is no significant drop in performance. |  |
| com_engineHz | Sets the games FPS cap, Bare in mind that setting this to a high number can cause some stability issues with the games physics. I’ve tested this up to 240 FPS. This issue should be resolved when the new physics engine is implemented. |  |
| cl_showfps | This shows the game FPS in the top right corner. The value accepted is between 0 - 1. | com_showfps |
| cl_autowepswitch | Toggles weapon switch on pickup. 0 is off 1 is on. | ui_autoswitch |
| cl_viewmodel | Toggles the weapon viewmodel. 0 disables viewmodel and 1 enables the viewmodel. | ui_showGun |
| cl_autoreload | Toggles auto reloading of weapons. 0 stops the weapon from auto reloading. 1 allows for the weapon to auto reload once it hits zero. | ui_autoReload |