# OVMF是什麼
我們在寫UEFI的程式時若手邊剛好沒有測試機台, 可以試試虛擬機器OVMF.<br>
本篇只探討如何在OVMF下驗證程式而已, 詳細的OVMF說明不在本篇討論範圍.<br>

## OVMF BIOS
OVMF也需要一個BIOS, 可從底下連結取得:<br>
[OVMF.fd](https://github.com/clearlinux/common/blob/master/OVMF.fd)
有興趣的話, 也可參考底下連結試著生產出ovmf.fd, 不過我個人實驗結果是缺少太多openssl的檔案, 因此轉用上方的連結取得.<br>
[How to build OVMF](https://github.com/tianocore/tianocore.github.io/wiki/How-to-build-OVMF)

