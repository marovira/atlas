#!/bin/bash

echo "Downloading test data..."
mkdir ./test/data/obj
curl https://casual-effects.com/g3d/data10/common/model/cube/cube.zip -o ./test/data/obj/cube.zip
curl https://casual-effects.com/g3d/data10/common/model/crytek_sponza/sponza.zip -o ./test/data/obj/sponza.zip
curl https://casual-effects.com/g3d/data10/common/model/teapot/teapot.zip -o ./test/data/obj/teapot.zip
echo "Test data download complete"
