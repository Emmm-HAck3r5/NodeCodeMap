const rf = require('fs');

var CodePage = {};
var pageArr = [];

// // TO FIX THE STRANGE 'DOUBLE FIRED BUG'
var dblfiredBan = false; 
// //

CodePage.loadCode = function(file, id){
    const code = rf.readFileSync(`../public/sample${id % 3 + 1}.cpp`, 'utf-8')
                   .replace(/ /g, '&nbsp;')
                   .replace(/</g, '&lt;')
                   .replace(/>/g, '&gt;')
                   .replace(/\r\n/ig, '<br/>');
    const fileTag = `sample${id}Cpp`;
    if( checkNotExist(pageArr, fileTag)){   
        insertPage(id, code);
        pageArr.push(fileTag);
        console.log(pageArr);
        $(`.CLOSE-${fileTag}`)
            .unbind()
            .on('click', closePage);
    }
    $(`#T-${fileTag} a`).tab('show');

    PR.prettyPrint();
};

function insertPage(id, code){
    const fileTag = `sample${id}Cpp`;
    const tabHTML = `
        <li role="presentation" id="T-${fileTag}"  style="display:inline-block;">
            <a href="#C-${fileTag}" aria-controls="C-${fileTag}" role="tab" data-toggle="tab">
                <p class="pagetext">sample${id % 3 + 1}.cpp</p>
                <button type="button" class="close CLOSE-${fileTag}" aria-label="Close"><span aria-hidden="true" class="CLOSE-${fileTag}">&times;</span></button>
            </a>
        </li>`;
    $('.codeTab').append(tabHTML)
    $('.pagetext').click( (e) => {
            // // TO FIX THE STRANGE 'DOUBLE FIRED BUG'
            if(dblfiredBan){
                dblfiredBan = false;
                return;
            }
            dblfiredBan = true;
            // //
            console.log('SHOW?');
            e.preventDefault();
            $(this).tab('show');
        });
    const contentHTML = `
        <div role="tabpanel" class="tab-pane fade" id="C-${fileTag}">
            <pre class="prettyprint">${code}</pre>
        </div>`;
    $('.codeContent').append(contentHTML);
    return fileTag;
}

function closePage(event){
    // // TO FIX THE STRANGE 'DOUBLE FIRED BUG'
    if(dblfiredBan){
        dblfiredBan = false;
        return;
    }
    dblfiredBan = true;
    // //
    const codeId = $(event.target).parent().parent().parent()
                        .attr('id')
                        .slice(2);
    const codeIdArrIndex = pageArr.findIndex( (e) => {
        return e === codeId;
    });

    let afterDestoryCodeId = pageArr[codeIdArrIndex + 1];
    let afterIn = codeIdArrIndex + 1;
    if(codeIdArrIndex + 1 >= pageArr.length){
        if(codeIdArrIndex - 1 < 0){ // if this page is the only
            console.log('There is no file!!DESTORY!!');
            $('.codePage').attr('style', 'display: none');
            D3Svg.resetSvg();
        }
        else{ // if there is no next one page
            console.log('TURN TO LAST ONE!!');
            afterDestoryCodeId = pageArr[codeIdArrIndex - 1];
            afterIn = codeIdArrIndex - 1;
        }
    }
    // // THERE IS A BUG!!
    if(afterDestoryCodeId !== undefined){
        console.log(pageArr);
        console.log(`#T-${afterDestoryCodeId} a`);
        $(`#T-${afterDestoryCodeId} a`).tab('show'); // next tab showed
    }
    // //

    // removing
    $(`#C-${codeId}`).remove();
    $(`#T-${codeId}`).remove();
    pageArr.splice(codeIdArrIndex, 1);
}

function checkNotExist(arr, item){
    let count = 0;
    for(const ele of arr){
        if(ele === item){
            count++;
        }
    }
    if(count >= 1){
        return false;
    }else{
        return true;
    }
}