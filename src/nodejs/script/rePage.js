const rf = require('fs');

var CodePage = {};
var pageArr = [];

CodePage.loadCode = function(filePath, id){
    const code = rf.readFileSync(`../public/sample${id % 3 + 1}.cpp`, 'utf-8')
                   .replace(/ /g, '&nbsp;')
                   .replace(/</g, '&lt;')
                   .replace(/>/g, '&gt;')
                   .replace(/\r\n/ig, '<br/>');
    const fileTag = `sample${id}`;
    const isFileExist = pageArr.includes(fileTag);
    if(isFileExist){
        moveToCertainPage(fileTag);
    }else{
        creatNewPage(fileTag, code);
    }
}

// // Move to the existed page
function moveToCertainPage(fileTag){
    // console.log('MOVE TO CERTAIN PAGE!!');
    $(`#TAB-${fileTag} a`).tab('show');
}
// //

// // Create a new page
function creatNewPage(fileTag, code){
    // console.log('CREAT NEW PAGE!!');
    insertTabHTML(fileTag);
    insertContentHTML(fileTag, code);
    pushAndShow(fileTag);
}

function insertTabHTML(fileTag){
    // console.log('INSERT TAB!!');
    const tabHTML = `
        <li id="TAB-${fileTag}">
            <a href="#CONTENT-${fileTag}" data-toggle="tab" class="tag" id="TAB-${fileTag}">${fileTag}.cpp</a>
            <a href="#" class="closebutton" id="RM-${fileTag}">&times;</a>    
        </li>`;
    $('.nav-tabs').append(tabHTML);
    $(`#TAB-${fileTag} a`)
        .click( (event) => {
            console.log('show?');
            event.preventDefault();
            const fileId = $(event.target).attr('id').slice(4);
            $(`.tab-pane`).removeClass('show');
            $(`#CONTENT-${fileId}`).addClass('show');
        });
    $(`#TAB-${fileTag} button`)
        .click(buttonRemoveEvent);
}
// // Remove Event
function buttonRemoveEvent(event){
    console.log('remove?');
    const thisFileTag = $(event.target).attr('id')
                            .slice(3);
    const nextFileTag = getNextTag(thisFileTag);
// console.log(`
// pageArr: ${pageArr},
// thisFileTag: ${thisFileTag},
// nextFileTag: ${nextFileTag}
// `); 
    if(nextFileTag === -1){
        destoryPage(thisFileTag);
        return;
    }
    removeSelf(thisFileTag);
    showNext(nextFileTag);
}
function getNextTag(thisFileTag){
    // console.log('search next tag?');
    const thisPosi = pageArr.indexOf(thisFileTag);
    if(thisPosi + 1 >= pageArr.length){
        if(thisPosi === 0){ // first element
            return -1;
        }else{ // last element but not first
            return pageArr[thisPosi - 1];
        }
    }else{ // mid element
        return pageArr[thisPosi + 1];
    }
}
function destoryPage(thisFileTag){
    // console.log('DESTORY!!');
    removeSelf(thisFileTag);
    D3Svg.resetSvg();
}

function removeSelf(thisFileTag){
    // console.log('remove self?');
    $(`#TAB-${thisFileTag}`).remove();
    $(`#CONTENT-${thisFileTag}`).remove();
    const tagIdArrIndex = pageArr.indexOf(thisFileTag);
    pageArr.splice(tagIdArrIndex, 1);
}

function showNext(nextFileTag){
    // console.log('show next?');
    $(`.tab-pane`).removeClass('show');
    $(`#CONTENT-${nextFileTag}`).addClass('show');
}
// //

function insertContentHTML(fileTag, code){
    // console.log('INSERT CONTENT!!');
    // let firstPageClass = '';
    // if (pageArr.length === 0){
    //     firstPageClass = 'in ';
    // }
    const contentHTML = `
        <div class="tab-pane" id="CONTENT-${fileTag}">
            <pre class="prettyprint">${code}</pre>
        </div>`;
    $('.tab-content').append(contentHTML);
}

function pushAndShow(fileTag){
    // console.log('PUSH AND SHOW!!');
    pageArr.push(fileTag);
    $(`.tab-pane`).removeClass('show');
    $(`#CONTENT-${fileTag}`).addClass('show');
    PR.prettyPrint();
}


// //