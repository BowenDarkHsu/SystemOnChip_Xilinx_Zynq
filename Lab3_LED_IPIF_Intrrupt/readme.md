# Lab3_LED_IPIF_Intrrupt
## 本次練習 將 硬體產生的中斷，安裝到CPU，進而可以在適當時機觸發 CPU執行中斷程式。
### 目標 : 延續 前一個題目 
* 利用CPU設定好 上、下限的計數器，可以讓計數器往上數，數到上限，再從下限數到上限，無限Loop。(功能同Lab2)
* 新增一顆按鈕，按下按鈕時，CPU執行中斷服務常式，
* 服務常式內容設定 : slv_reg3 設定 給一個值，LED並放棄原來的計數，依據 slv_reg3 的值，做規律的閃爍功能。

