#! /bin/sh

echo "Building icons..."
echo
source python/bin/activate
python ./imgconvert.py -i ../docs/pumpOff.png -n pumpOff -o ../lib/icons/pumpOff.h
python ./imgconvert.py -i ../docs/pumpLow.png -n pumpLow -o ../lib/icons/pumpLow.h
python ./imgconvert.py -i ../docs/pumpHigh.png -n pumpHigh -o ../lib/icons/pumpHigh.h

python ./imgconvert.py -i ../docs/lightOff.png -n lightOff -o ../lib/icons/lightOff.h
python ./imgconvert.py -i ../docs/lightOn.png -n lightOn -o ../lib/icons/lightOn.h

python ./imgconvert.py -i ../docs/highHeatOff.png -n highHeatOff -o ../lib/icons/highHeatOff.h
python ./imgconvert.py -i ../docs/highHeatOn.png -n highHeatOn -o ../lib/icons/highHeatOn.h

python ./imgconvert.py -i ../docs/lowHeatOff.png -n lowHeatOff -o ../lib/icons/lowHeatOff.h
python ./imgconvert.py -i ../docs/lowHeatOn.png -n lowHeatOn -o ../lib/icons/lowHeatOn.h

python ./imgconvert.py -i ../docs/filterOff.png -n filterOff -o ../lib/icons/filterOff.h
python ./imgconvert.py -i ../docs/filterOn.png -n filterOn -o ../lib/icons/filterOn.h