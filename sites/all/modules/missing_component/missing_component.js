$(document).ready(function(){
    if($('.pmissing-type-div #edit-missing-type-0').attr('checked')){
        $('.rpmissing_comment-div').css({'display':'none'});
    }
   $('.report_mail').css({'display':'none'});
	$('.report_missing_comp #edit-report-0, .report_missing_comp #edit-report-1').change(function(){
		if($('.report_missing_comp #edit-report-0').attr('checked')){
			$('.report_mail').css({'display' : 'none'});
			$('.report_mail').val('');
		}else{
			$('.report_mail').css({'display' : 'block'});
		}
	});



    $('.pmissing-type-div #edit-missing-type-0, .pmissing-type-div #edit-missing-type-1').change(function(){
		if($('.pmissing-type-div #edit-missing-type-0').attr('checked')){
			$('.rpmissing_comment-div').css({'display' : 'none'});
			$('.rpmissing_comment-div').val('');
		}else{
			$('.rpmissing_comment-div').css({'display' : 'block'});
		}
	});
});
function validate_report(){
    if($('.pmissing-type-div #edit-missing-type-1').attr('checked') && $('#edit-email').val() == ''){
	    alert('Please give valid remarks');
	    return false;
    }
    if($('.report_missing_comp#edit-report-1').attr('checked') && $('#edit-email').val() == ''){
	    alert('Please enter your mail id');
	    return false;
    }
    else if($('.report_missing_comp#edit-report-1').attr('checked') &&IsEmail($('#edit-email').val())==false){
		alert("invalid email id");
		return false;
	}
}
function IsEmail(email) {
	var regex = /^([a-zA-Z0-9_\.\-\+])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
	if(!regex.test(email)) {
		return false;
	}
	else
	{
		return true;
	}
}
