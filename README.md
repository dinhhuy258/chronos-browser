# chronos-browser
I do this project just for studying purpose

##Browser is under construction
#Features was implemented:
- Adblock (avoid being detected by website) 
- Popup blocker

#TODO
Currently popup blocker feature was developed in a bad design. Because popup blocker happends in renderer so I can not use chrome setting value. Currently I saved the popup blocker setting value in the config file. And in renderer I send the sync message to get_config_msg_helper to read it. I still don't understand why I can't open the config file in renderer so I decided send the sync message and implemented get_config_msg_helper to read it in browser.
In the future I will refactor this feature when I understand chromium system clearly
