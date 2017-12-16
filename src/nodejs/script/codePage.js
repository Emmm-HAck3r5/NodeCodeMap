const rf = require('fs');

var CodePage = {};

CodePage.loadCode = function(file, id){
    console.log('LOAD CODE SUCCESSFUL!!');
    const code = rf.readFileSync(`../public/sample${id % 3 + 1}.cpp`, 'utf-8')
                   .replace(/ /g, '&nbsp;')
                   .replace(/</g, '&lt;')
                   .replace(/>/g, '&gt;')
                   .replace(/\r\n/ig, '<br/>');

    const fileTag = `sample${id}Cpp`;
    const tabHTML = `
        <li role="presentation" id="T-${fileTag}">
            <a href="#${fileTag}" aria-controls="${fileTag}" role="tab" data-toggle="tab">
                sample${id % 3 + 1}.cpp
                <button type="button" class="close fileTag" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            </a>
        </li>`;
    $('.codeTab').append(tabHTML)
        .click( (e) => {
            e.preventDefault();
            $(this).tab('show');
        });
    const contentHTML = `
        <div role="tabpanel" class="tab-pane fade" id="${fileTag}">
            <code class="prettyprint">
                ${code}
            </code>
        </div>`;
    $('.codeContent').append(contentHTML);
    $(`#T-${fileTag} a`).tab('show');
    PR.prettyPrint();
    $('.close').click( (event) => {
        $(event.target).parent().parent().remove();
    })
};