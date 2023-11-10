# OVMF是什麼
我們在寫UEFI的程式時若手邊剛好沒有測試機台, 可以試試虛擬機器OVMF.<br>
本篇只探討如何在OVMF下驗證程式而已, 詳細的OVMF說明不在本篇討論範圍.<br>

## OVMF BIOS
OVMF也需要一個BIOS, 可從底下連結取得:<br>
[OVMF.fd](https://github.com/clearlinux/common/blob/master/OVMF.fd)
有興趣的話, 也可參考底下連結試著生產出ovmf.fd, 不過我個人實驗結果是缺少太多openssl的檔案, 因此轉用上方的連結取得.<br>
[How to build OVMF](https://github.com/tianocore/tianocore.github.io/wiki/How-to-build-OVMF)

## Install qemu
在WSL的環境下執行底下指令安裝qemu, 以便執行OVMF
```
sudo apt install qemu
sudo apt install qemu-system-x86
```
我實際2個都安裝, 不過應該後者就可以了.<br>

## 執行OVMF
以我的環境來說, 我在/HOME/scr下建立了qemu的資料夾, 把ovme.fd改為bios.bin複製到qemu下, 並且在qemu中建立了hda-contents資料夾,<br> 
如下圖:<br>

執行的指令如下:<br>
```
qemu-system-x86_64 -L . --bios ./bios.bin -net none -hda fat:rw:hda-contents
```
