$(document).ready(function(){
	webroot = 'http://'+location.hostname+'/';
	loading_image = "<img src='http://"+location.hostname+"/ajax-loader.gif' />";
	$('.cd-foss-category').change(function(){
		foss = $(this).val();	
		$.ajax({
			type : 'POST',
			url : webroot + "cd_get_lang",
			data : {
				'foss' : foss
			},
			beforeSend: function() {
			    field_data = $('.cd_lang_div').html();
			    $('.cd_lang_div').html(loading_image);
			},
			success : function(data){
				$('.cd_lang_div').html(field_data);
				var html_data = "";
				output = JSON.parse(data);
				if(output){
					for (var i=0; i < output.length; i++){
						//html_data += "<option value='"+ output[i].language +"'>" + output[i].language + "</option>";	
						html_data += "<option value='"+ output[i][0] +"'>" + output[i][0] + " (" + output[i][1] +")" + "</option>\n";
					}
					$('.cd-language').html(html_data);
				}else{
					alert('Somthing wrong, Please refresh page');
				}
			}
		});
	});
});
