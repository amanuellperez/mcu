#!/bin/bash

ttf2txt.py -s8 dogica.ttf
ttf2txt.py -s8 dogicabold.ttf
ttf2txt.py -s8 dogicapixelbold.ttf
ttf2txt.py -s8 dogicapixel.ttf

ttf2txt.py -s9 DePixelBreit.ttf
ttf2txt.py -s9 DePixelBreitFett.ttf
ttf2txt.py -s9 DePixelHalbfett.ttf
ttf2txt.py -s9 DePixelIllegible.ttf
ttf2txt.py -s9 DePixelKlein.ttf
ttf2txt.py -s9 DePixelSchmal.ttf

ttf2txt.py -s10 mai10.ttf

ttf2txt.py -s11 RetroGaming.ttf

ttf2txt.py -s16 alagard.ttf
ttf2txt.py -s16 Minecraft.ttf
ttf2txt.py -s16 PerfectDOSVGA437.ttf
ttf2txt.py -s16 PerfectDOSVGA437Win.ttf
ttf2txt.py -s16 rainyhearts.ttf
ttf2txt.py -s20 HomeVideo_Regular.ttf
ttf2txt.py -s20 upheavtt.ttf

ttf2txt.py -s21 VCR_OSD_MONO_1.001.ttf

# Para generar los .h:
# for f in *.txt
# do 
#   ttf_txt2bin.py $f
# done
# mv *.h ../..
