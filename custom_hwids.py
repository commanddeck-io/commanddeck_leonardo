Import("env")

board_config = env.BoardConfig()

board_config.update("build.hwids", [
["0x1209", "0xF0DA"]
])

board_config.update("build.usb_product", "Command Deck")
board_config.update("build.usb_manufacturer", "Hyades")
