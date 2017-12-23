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
            $('.searchbox').attr('style', 'display: none;');
        }
    })
});