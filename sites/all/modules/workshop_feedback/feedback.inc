<?php

function workshop_feedback(){
		global $user;
		$filter = unserialize(base64_decode($_REQUEST['filter']));
		$filter_query = '';
		$selected_val = array();
		if($filter['foss'] != ''){
			$filter_query .= " and wr.foss_category = '".$filter['foss']."'";
			$selected_val['foss'] = $filter['foss'];
		}
		if($filter['state'] != ''){
			$filter_query .= " and wr.academic_code like '".$filter['state']."%'";
			$selected_val['state'] = $filter['state'];
		}
		$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac, workshop_feedback wf where wr.status=2 and ac.academic_code=wr.academic_code and ac.academic_code != 'MAH-00029' wr.workshop_code = wf.workshop_code";
		$count_query = "select count(*) as count from workshop_requests wr, academic_center ac, workshop_feeedback wr where wr.status=2 and ac.academic_code=wr.academic_code  and ac.academic_code != 'MAH-00029'";
		$tmp_query_param = "";
		if(!isset($selected_val['state'])){
			foreach($state_code_arr as $st_code){
				if($tmp_query_param != ""){
						$tmp_query_param .= " or ";
				}
				$tmp_query_param .=	"wr.academic_code like '".$st_code."%'";
			}
			$tmp_query_param = ' and ('.$tmp_query_param.')';
		}
		$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac, workshop_feedback wf where wr.status=2 ".$tmp_query_param.$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code!='MAH-00029' and wr.workshop_code = wf.workshop_code GROUP BY wr.workshop_code";
		$count_query = "select count(*) as count from workshop_requests wr, academic_center ac, workshop_feedback wf where wr.status=2 ".$tmp_query_param.$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code!='MAH-00029' and wr.workshop_code = wf.workshop_code GROUP BY wr.workshop_code";
		$participant_count_query = "select sum(wd.no_of_participants) as count from workshop_requests wr, academic_center ac, workshop_details wd, workshop_feedback wf where wr.status=2 ".$tmp_query_param.$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code!='MAH-00029' and wd.workshop_code=wr.workshop_code and wr.workshop_code = wf.workshop_code GROUP BY wr.workshop_code";
		$header = array(
			array('data'=>'Workshop Code', 'field'=>'wr.id', 'sort'=>'desc'),
			array('data'=>'Institution', 'field'=>'ac.institution_name'),
			array('data'=>'State', 'field'=>'ac.state_code'),
			array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'asc')
		);
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		//var_dump($query);
		$result = pager_query($query , 10, 0, $count_query);
		$count = db_query($count_query);
		$count = db_fetch_object($count);
		$participant_count = db_query($participant_count_query);
		$participant_count = db_fetch_object($participant_count);
		db_set_active('default');
		$row = array();
		$sno = 1;
		$query = "select name,code from workshop_info.states";
		$result_state = db_query($query);
		while($row = db_fetch_object($result_state)){
			$state[$row->code] =  $row->name;
		}
		$states_array = $state;
		while ($item = db_fetch_object($result)){
			$row[] = array($item->workshop_code, $item->institution_name, $states_array[$item->state_code], $item->cfm_wkshop_date." ".$item->cfm_wkshop_time, l('View', 'workshops/view_workshop/'. $item->id, array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
		}
		$output = "<table><tr><th><b>Total no of workshops : ".$count->count."</b></th><th><b>Total no of participant : ".$participant_count->count."</b></th></tr></table></b>";
		$output .= drupal_get_form('workshop_filter_form', $selected_val);
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 10);
		}
		return $output;
	}
	function workshop_filter_form($form_state, $selected_val) {
		global $user;
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		drupal_add_css($path . '/workshops.css', 'module');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js("$(document).ready(function(){ new datepickr('edit-from-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		drupal_add_js("$(document).ready(function(){ new datepickr('edit-to-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$fquery = "select name from foss_categories where workshop=1 order by name ASC";
		$foss_categories = db_query($fquery);
		$lquery = "select pref_language from workshop_requests where status=2 group by pref_language";
		$squery = "select states from resource_person where user_uid='" . $user->uid."'";
		db_set_active('workshop_info');
		$lresult = db_query($lquery);
		$sresult = db_query($squery);
		db_set_active('default');
		$foss_category[''] = 'All';
		while($row = db_fetch_object($foss_categories)) {
			$foss_category[$row->name] = human_text($row->name);
		}
		$languages[''] = 'All';
		while($row = db_fetch_object($lresult)) {
			$languages[$row->pref_language] = $row->pref_language;
		}
		/* select states */
		$query = "select name,code from workshop_info.states";
		$result = db_query($query);
		while($row = db_fetch_object($result)){
			$state[$row->code] =  $row->name;
		}
		$states_array = $state;
		# get current resource person name
		
		$row = db_fetch_object($sresult);
		$state_codes = explode(",", $row->states);
	
		$states = array();
		$states[''] = "All";
		foreach($state_codes as $st_code){
			$states[$st_code] = $states_array[$st_code];
		}
		
		/* check collapse */
		$attr = 1;
		if(count($selected_val) > 0)
			$attr = 0;
		$form['filter'] = array(
		 	'#type' => 'fieldset',
		    '#title' => t('Filter'),
		    '#weight' => 5,
		    '#collapsible' => FALSE,
		    '#collapsed' => $attr,
		);
		$form['filter']['foss'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category'),
			'#attributes' => array('id' => 'resource_person_state'),
			'#options' => $foss_category,
			'#default_value' => $selected_val['foss'],

		);
		$form['filter']['state'] = array(
			'#type' => 'select',
			'#title' => t('State'),
			'#attributes' => array('id' => 'resource_person_state'),
			'#options' => $states_array,
			'#default_value' => $selected_val['state'],

		);
		$form['filter']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
	
		return $form;
	}
	
/*	function workshop_filter_form_validate($form, &$form_state){
	if ($form_state['values']['from_date'] != '' && $form_state['values']['to_date'] != '' && ($form_state['values']['from_date'] > $form_state['values']['to_date']))
		form_set_error('academic_code', t('To date should not less that from date '.$form_state['values']['from_date']));
  		return;
	}*/
	function workshop_filter_form_submit($form, &$form_state){
	
		// filter
		$params = array('foss' => $form_state['values']['foss'], 'language' => $form_state['values']['language'],  'state' => $form_state['values']['state'],'type' => $form_state['values']['type'], 'from_date' => $form_state['values']['from_date'], 'to_date' => $form_state['values']['to_date']);
		$filter =  array('filter' => base64_encode(serialize($params)));
		$redirect = $_REQUEST['q'];
		drupal_goto($path = $redirect, $query = $filter, $fragment = NULL, $http_response_code = 302);
	}
?>
