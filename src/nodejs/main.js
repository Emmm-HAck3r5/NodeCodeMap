const {app, BrowserWindow} = require('electron');
const path = require('path');
const url = require('url');

// 將這個 window 物件記在全域變數裡。
// 如果你不這麼做，這個視窗在 JavaScript 物件被 GC 後就會被自動關閉。
let win;

function createWindow () {
  // 建立瀏覽器視窗。
  win = new BrowserWindow({
    fullscreen: true
  });

  // 並載入應用程式的 index.html。
  win.loadURL(url.format({
    pathname: path.join(__dirname, 'index.html'),
    protocol: 'file:',
    slashes: true
  }));

  // 打開 DevTools。
  win.webContents.openDevTools();

  // 視窗關閉時會觸發。
  win.on('closed', () => {
    // 拿掉 window 物件的參照。如果你的應用程式支援多個視窗，
    // 你可能會想存成陣列，現在該是時候清除相關的物件了。
    win = null;
  });
}

// 這個方法在 Electron 初始化完成，準備好建立瀏覽器視窗時會被叫用。
// 有些 API 只能在這個事件發生後才能用。
app.on('ready', createWindow);

// 在所有視窗都關閉時結束程式。
app.on('window-all-closed', () => {
  // 在 macOS 裡，普遍的作法是將應用程式及選單列繼續留著，直到使用者按了 Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  // 在 macOS 裡，一般會在使用者按了 Dock 圖示且沒有其他視窗開啟的情況下，
  // 重新在應用程式裡建立視窗。
  if (win === null) {
    createWindow();
  }
});