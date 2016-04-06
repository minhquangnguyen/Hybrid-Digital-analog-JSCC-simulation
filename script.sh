#! /bin/sh
echo `amrnb-encoder MR67 f.raw 67.amr`

echo `./output 67.amr u67.amr e67.raw e1.raw -u`
echo `./output 67.amr _67.amr e67.raw e1.raw`

echo `amrnb-decoder u67.amr u67.raw`
echo `amrnb-decoder _67.amr _67.raw`

echo `./PESQ +8000 f.raw u67.raw`
echo `./PESQ +8000 f.raw _67.raw`

echo `./finish 67.raw e1.raw ue67.raw`
echo `./finish _67.raw e1.raw _e67.raw`

echo `./PESQ +8000 f.raw ue67.raw`
echo `./PESQ +8000 f.raw _e67.raw`
