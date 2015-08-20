/* forum */
$( document ).ready(function() {
	webroot = 'http://www.spoken-tutorial.org/';
	loading_image = "<img src='http://www.spoken-tutorial.org/ajax-loader.gif' />";
	
	$('#edit-state').on("change", function(){

		if($('#edit-state').val() != ''){
			$.ajax({
				type : 'POST',
				url : webroot + "filter/get_state",
				data : {
					'state' : $('#edit-state').val() 
				},
				beforeSend: function() {
					field_data = $('#edit-foss-category').html();
					$('#edit-foss-category').html(loading_image);
				},
				success : function(data){
					$('#edit-foss-category').html(field_data);
					output = JSON.parse(data);
					if(output){
						html_data = "";
						count = 0;
						for (var i=0; i < output.length; i++) {
							html_data += "<option value='"+ output[i][0] +"'>" + output[i][0] + "</option>\n";
							count++;
						}
						html_data = "<option value=''>All FOSS Category</option>\n"+html_data;
						if(count > 1) {
							html_data = "<option value='1'>Select FOSS Category</option>\n"+html_data;
						}
						$('#edit-foss-category').html(html_data);
					}else{
						alert('Error fetching languages, please refresh the page and try again');
					}
				}
			});
		}
	});
    $(".toggle-content").hide();
	$(".toggle").click(function () {
		$(this).toggleClass("toggled");
		$(".toggle-content").slideToggle("slow");
	});
 }); 

