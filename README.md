<div align="center">

# 112-2 Project 2 - For The King (clone)

### For The King (clone)

</div>


## Member
* B11215005 温冠華（組長）
* B11215063 吳盛瑋
* B11215039 陳亮宏
* B11215045 謝睿哲

## Quick Start
直接打開專案，直接點執行即可遊玩。

## Contribution

### 温冠華
1. 專案初始架構
2. layout 排版
    1. 將 `std::stringstream` 全部改成 `setPosition` 輸出
    2. 修改戰鬥畫面排版
    3. 修改走路畫面排版
    4. 修改購買畫面排版
3. 新增使用者狀態欄
4. 擴大 console 尺寸 & buffer 大小
5. refactor 所有檔案，消滅 spaghetti code (但最後還是有)
6. 新增商店功能
7. 新增購買功能
8. 新增背包功能
9. 新增裝備穿戴功能 和 裝備能力值
10. 新增道具使用功能
11. 
4. fix bugs & chore:
    1. 將錯誤的程式碼移到正確位置
    2. 使用 `std::shared_ptr` 將裝備能正確儲存至包包中（fix 子物件遺失）
    3. 人死了，繼續動
    4. 怪物死了，還在原位
    5. 生命值出現負數

### 吳盛瑋

1. 戰鬥事件 Handling
2. 攻擊 layout
    1. 玩家資訊顯示
    2. 選單選擇介面
    3. resultLog 輸出
3. 舊版 Refactor 製作 layout 獨立物件 （後退版）

### 陳亮宏
1. 舊版印出地圖及事件觸發
2. 移動(已刪除使用專注移動)
3. 隨機事件
4. 穿戴裝備新增技能及Buff
5. fix bugs & chore:
   1. 限制開始遊戲的畫面大小
   2. 往邊界或牆移動消耗movepoint
   3. 小部份顯示更新
### 謝睿哲
1. 新增戰鬥功能
2. 新增主動、被動以及buff功能
3. fix bugs & chore:
    1. 更換武器後改變攻擊模式
