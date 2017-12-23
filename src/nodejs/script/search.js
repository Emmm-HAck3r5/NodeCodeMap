var ipcRenderer = require('electron').ipcRenderer;

$(document).ready( () => {
    $('.searchbox').bind('keydown', (event) => {
        const key = event.which;
        if (key == 13) {
            const value = $('.searchbox').val();
            console.log(value);
            const jsonfile = ipcRenderer.sendSync('getpath', value);
            console.log(jsonfile);
            D3Svg.initSvg(jsonfile);
            $('.search-container').attr('style', 'display: none;');
        }
    })
});

const openDialogBtn=$("#open-dialog");
openDialogBtn.click((event)=>{
    const reply=ipcRenderer.sendSync('askforopendialog');
})

const submitBtn=$("#submit");
submitBtn.click((event)=>{
            const value = $('.searchbox').val();
            console.log(value);
            const jsonfile = ipcRenderer.sendSync('getpath', value);
            console.log(jsonfile);
            D3Svg.initSvg(jsonfile);
            $('.search-container').attr('style', 'display: none;');
})