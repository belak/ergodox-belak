# Belak's Ergodox Layout

This has been based off of [emacs\_osx\_dk](https://github.com/jackhumbert/qmk_firmware/tree/master/keyboards/ergodox/keymaps/emacs_osx_dk)
from the main qmk repo. However, I've taken some of the ideas for the thumbs
from [dvorak\_emacs](https://github.com/jackhumbert/qmk_firmware/tree/master/keyboards/ergodox/keymaps/dvorak_emacs)
and tweaked it a bit based on the keycaps I have.

## Instructions

1. Clone the main qmk repo
2. Clone this to `$QMK/keyboards/ergodox/keymaps/belak`
3. Run `make ergodox-belak-teensy`

## Changelog

Fifth Revision

* Change layer keys to tap-dance keys which cycle through additional layers

Fourth Revision

* Remove media layer
* Add a layer which swaps control and gui on the thumb keys.
* Add some basic code to save settings to the eeprom
* Save the state of the keys swapped in the thumb in the eeprom

Third Revision

* Add numpad layer and remove numpad from symbols layer
* Disable media layer
* Add arrow keys on ijkl to the symbols layer
* Replace ALT on held enter and held delete with GUI (for better OSX
  compatibility, as there's already an ALT key relatively close)
* Replace keys above enter and delete with temporary layer switch buttons not
  matching the other layer switch for that hand.
* Reindent and space out most of the layer definitions

Second Revision

* Clean up definitions to make differences between layers easier to see
* Remove old LCD code
* Add new LCD code based on fredizzimo's branch

First Revision

* Reverse grave and escape

Initial Version

* Copy from emacs\_osx\_dk
* "Fix" right alt
* Change thumb keys to match default layout (backspace, delete, enter, space)
* Add modifiers to thumb keys (ctrl to backspace and space, alt to delete and
  enter)
* Replace the RAlt below the brackets with LGui and RGui
* Remove LCtrl and RCtrl from the keys above shift
* Add browser forward, and move browser back
* "Fix" the order of volume keys
