$(document).ready(function(){
    $('.form-wrapper').slideToggle();

    /* Reply/Close button Action */
    $(".btn-reply a").click(function(){
        var clicked = $(this).attr("class");
        //alert('#'+clicked);
        $('#'+clicked).slideToggle();
        console.log($(this).html());
        if ($(this).html() == "Reply") {
            $(this).html("Close");
        } else {
            $(this).html("Reply");
        }
    });
    
    /* Delete a post */
    $(".delete").click(function(e){
        sure = confirm("Are you sure ?");
        console.log(sure);
        if (sure) {
            window.location.href = $(this).attr("href");
        }
        else {
            console.log("nopees");
        }
        e.preventDefault();
    });
});
