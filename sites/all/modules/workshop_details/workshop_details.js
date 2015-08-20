$(document).ready(function(){
webroot = "http://www.spoken-tutorial.org/";
loading_image = "<img src='http://www.spoken-tutorial.org/ajax-loader.gif' />";
$('#wrkshp_code').change(function(){
		var wkshpcode = $('#wrkshp_code').val();
		$.ajax({
			type : 'POST',
			url : webroot + "attendance-register/get_wkshop_code",
			//dataType: "json",
			data : {
				'wkshpcode' : $('#wrkshp_code').val()
			},
			success : function(data){
				output = JSON.parse(data);
				//alert(output);
				console.log(output[0].no_of_participants);
				if(typeof output[0].no_of_participants === 'undefined')
				{
					$('#edit-student-count').val('');
					$('#edit-student-count').attr("placeholder", "Please enter the no. of students attended workshops");
					$('#edit-student-count').removeAttr("readonly");
				}else{
				    $('#edit-student-count').val(output[0].no_of_participants);
				    $('#edit-student-count').attr("readonly","readonly");
				}
				$('#edit-institution-name').val(output[0].institution_name);
				$('#edit-institution-name').attr("readonly","readonly");
				$('#edit-foss').val(output[0].foss_category);
				$('#edit-foss').attr("readonly","readonly");
				$('#edit-organiser-name').val(output[0].organiser_name);
				$('#edit-organiser-name').attr("readonly","readonly");
			},
			error: function (xhr, textStatus, err) {
				alert('Error: ' + err);
			}
		});
	});
	$more = $("#add_details");
	var count = 1;
	$('#add_new_details').click(function(e){
		//$('#add_details').clone().insertAfter("#add_details");
		//$("#add_details").clone().attr('add_details', 'add_details'+ cloneCount++).insertAfter("#add_details");
		$dupe = $("#add_details").clone();
		$("#add_details").attr("id", "add_details" + count);
		$("#add_details"+count).css("clear", "both");
		$dupe.find("input[name='student_firstnm']").attr("id", "edit-student-firstnm" + count);
		$dupe.find("input[name='student_lastnm']").attr("id", "edit-student-lastnm" + count);
		$dupe.find("input[value='Male']").attr("id", "edit-gender-Male" + count);
		$dupe.find("input[value='Female']").attr("id", "edit-gender-Female" + count);
		$dupe.insertBefore($more);
		count++;
	});
	$('#delete').click(function(e){
		$('#add_details').remove();
	});
});
