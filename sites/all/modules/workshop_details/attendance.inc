<?php
function attendance_form($form_state)
{
	global $user;
	$path = drupal_get_path('module', 'workshop_details');
	drupal_add_js($path . '/workshop_details.js');
	$form = array();
	$tutorial_name = array('' => '-- Select --');
	$query = "select workshop_code,id from workshop_info.workshop_requests where organiser_id='".$user->uid."' and (status = 2 or status = 1) ORDER BY id ASC";	
	$result = db_query($query);
	$workshop_code = array('' => '-- Select --');
	while($row = db_fetch_object($result)){
		$workshop_code[$row->workshop_code] =  $row->workshop_code;
	}
	$form['workshop_code'] = array(
		'#type' => 'select',
		'#title' => t('Workshop Code'),
		'#options' => $workshop_code,
		'#attributes' => array('id' => 'wrkshp_code'),
	);
	$form['student_count'] = array(
		'#type' => 'textfield',
		'#title' => t('No. of students attended workshop'),
		'#required' => TRUE,
	);
	$form['institution_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Institution Name'),
		'#required' => TRUE,
	);
	$form['foss'] = array(
		'#type' => 'textfield',
		'#title' => t('FOSS'),
		'#required' => TRUE,
	);
	$form['organiser_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Organiser Name'),
		'#required' => TRUE,
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}
function attendance_form_validate($form, &$form_state)
{
	if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['student_count']))
	{
		form_set_error('no_of_participants', t('Invalid participants count'));
	}
}
function attendance_form_submit($form, &$form_state)
{
	global $base_url;
    $query = "update workshop_requests set status=2 where workshop_code='".$form_state['values']['workshop_code']."'";
    db_set_active('workshop_info');
    db_query($query);
                
	$query = "insert into  workshop_details (workshop_code, no_of_participants) values('".$form_state['values']['workshop_code']."', '".$form_state['values']['student_count']."')";
	db_set_active('workshop_info');
	db_query($query);
	unset($_REQUEST['destination']);
	unset($form['#redirect']); 
	//$path = drupal_get_path('module', 'workshop_details');
	$form_state['redirect']  = $base_url.'/attendance-register/student_detail/'.base64_encode($form_state['values']['workshop_code']).'/'.base64_encode($form_state['values']['student_count']);
}

function student_detail_form($form_state)
{
	$form = array();
	$workshop_code = base64_decode(arg(2));
	$no_of_participants = base64_decode(arg(3));
	
	$form['workshop_code'] = array(
                '#type' => 'hidden',
        	'#value' => $workshop_code,
        );
        $form['no_of_participant'] = array(
                '#type' => 'hidden',
        	'#value' => $no_of_participants,
        );
	$count = 0;
	$query = "select * from workshop_info.student_detail where workshop_code='".$workshop_code."'";
	$result = db_query($query);
	while($row = db_fetch_object($result))
	{
		$firstname[$count] = $row->first_name;
		$lastname[$count] = $row->last_name;	
		$gender[$count] = $row->gender;
		$count++;
	}
	$path = drupal_get_path('module', 'workshop_details');
        drupal_add_css($path . '/workshop_details.css');
	for ($x=0; $x<$no_of_participants; $x++) {
		$form['first_name_'.$x] = array(
			'#type' => 'textfield',
			'#attributes' => array('placeholder' => 'First name'),
			'#size' => 20,
			'#default_value' => $firstname[$x],
			'#prefix' => '<div id="stu_det"><div id="student_name"><div id="first_name">',
			'#suffix' => '</div>',
		);
		$form['last_name_'.$x] = array(
			'#type' => 'textfield',
			'#size' => 20,
			'#attributes' => array('placeholder' => 'Last name'),
			'#default_value' => $lastname[$x],
			'#prefix' => '<div id="last_name">',
			'#suffix' => '</div></div>',
		);
		$form['gender_'.$x] = array(
			'#type' => 'radios',
			//'#title' => t('Gender'),
			'#options' => array(
		  	  'Male'   => t('Male'),
			  'Female' => t('Female'),
			),
			'#default_value' => $gender[$x],
			'#prefix' => '<div id="gender">',
			'#suffix' => '</div></div>',
		);
		if($firstname[$x] != null){
			$form['first_name_'.$x]['#attributes'] = array('readonly' => 'readonly');
		}
		if($lastname[$x] != null){
			$form['last_name_'.$x]['#attributes'] = array('readonly' => 'readonly');
		}
		if($gender[$x] != null){
                        $form['gender_'.$x]['#attributes'] = array('readonly' => 'readonly');
                }
		}
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
                	'#prefix' => '<div id="stu_submit">',
		        '#suffix' => '</div>',
		);
	return $form;
}
function student_detail_form_submit($form, &$form_state)
{	
	drupal_set_message("The student details has been saved. If workshop is completed then click here to download certificare.");
	for ($x=0; $x<$form_state['values']['no_of_participant']; $x++) {
	if(($form_state['values']['first_name_'.$x] != null)/*&&($form_state['values']['last_name_'.$x] != null)*/&&($form_state['values']['gender_'.$x] != null))
	{
	    $query = "insert into  student_detail (workshop_code, first_name, last_name, gender) values('".$form_state['values']['workshop_code']."', '".mysql_escape_string($form_state['values']['first_name_'.$x])."', '".mysql_escape_string($form_state['values']['last_name_'.$x])."','".$form_state['values']['gender_'.$x]."')";
            db_set_active('workshop_info');
            db_query($query);
	}
        }
}
function list_of_students($id = NULL)
{
	if($id){
        	$query = "select * from {workshop_requests} where id=".$id;
                db_set_active('workshop_info');
                $result = db_query($query);
                db_set_active('default');
                if($result){
                        $item = db_fetch_object($result);
                        $output = '<table width="100%" cellpadding="6">';
                        $output .= '<tr><td><b>Workshop Code</b></td><td><b>'.$item->workshop_code.'</b></td></tr>';
                        $output .= '<tr><td><b>Foss Category</b></td><td><b>'.human_text($item->foss_category).'</b></td></tr>';
                        $output .= '<tr><td><b>Language</b></td><td><b>'.$item->pref_language.'</b></td></tr>';
                        $output .= '<tr><td><b>Date & Time</b></td><td><b>'.$item->cfm_wkshop_date.' &nbsp;'.$item->cfm_wkshop_time.'</b></td></tr>';
                        $query = "select * from  {workshop_details} where workshop_code='".$item->workshop_code."'";
                        db_set_active('workshop_info');
                        $result = db_query($query);
                        db_set_active('default');
                        $ws_detail = db_fetch_object($result);
                        $output .= '<tr><td><b>Participants Count</b></td><td><b>'.$ws_detail->no_of_participants.'</b></td></tr>';
                        $output .= '<tr><td colspan="2"><hr /></td></tr>';
                        $query = "select * from {academic_center} where academic_code='".$item->academic_code."'";
                        db_set_active('workshop_info');
                        $result = db_query($query);
                        db_set_active('default');
                        $coll_detail = db_fetch_object($result);
                        $output .= '<tr><td><b>State Code</b></td><td><b>'.$coll_detail->state_code.'</b></td></tr>';
                        $output .= '<tr><td><b>Academic Center Code</b></td><td><b>'.$coll_detail->academic_code.'</b></td></tr>';
                        $output .= '<tr><td><b>Institution Name</b></td><td><b>'.$coll_detail->institution_name.'</b></td></tr>';
                        $output .= '<tr><td><b>Department</b></td><td><b>'.$item->department.'</b></td></tr>';
                        $output .= '<tr><td><b>Street</b></td><td><b>'.$coll_detail->street.'</b></td></tr>';
                        $output .= '<tr><td><b>City</b></td><td><b>'.$coll_detail->city.'</b></td></tr>';
                        $output .= '<tr><td><b>Pin Code</b></td><td><b>'.$coll_detail->pincode.'</b></td></tr>';
                        $output .= '</table>';
                        $output .= '<h2>Participants Details</h2>';
                        $query = "select sd.id, sd.first_name , sd.last_name from student_detail sd where sd.workshop_code='".$item->workshop_code."'";
			db_set_active('workshop_info');
                        $result = db_query($query);
                        db_set_active('default');
                        $output .= '<table width="100%" cellpadding="6"><tr><th>User name</th><th>Action</th></tr>';
                        while($fd = db_fetch_object($result)){
                                $output .= '<tr><td>'.$fd->first_name.' '.$fd->last_name."</td><td>".l('Download Certificate', 'attendance-register/download/'.$item->workshop_code.'/'.$fd->id, array('attributes' => array('target' => '_blank', 'title' => 'Download Certificate')))."</td></tr>";
                        }
                        $output .= '</table>';
                        return $output;
                }else {
                }
        }else{
                return "<p>Sorry, no data found.</p>";
        }
}

?>
