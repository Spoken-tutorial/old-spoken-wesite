<?php
	function add_new_testimonial_form() {
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		
		$form['original_body'] = array(
			'#type' => 'textarea',
			'#title' => t('Original Content'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'original_body', 'rows' => '20')
		);
		
		$form['modified_body'] = array(
			'#type' => 'textarea',
			'#title' => t('Modified Content'),
			'#attributes' => array('class' => 'modified_body', 'rows' => '20')
		);
		
		$form['source'] = array(
			'#type' => 'textfield',
			'#title' => t('Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'source')
		);
		
		$form['source_link_title'] = array(
			'#type' => 'textfield',
			'#title' => t('Source Link Title'),
			'#attributes' => array('class' => 'source_link_title')
		);
		
		$form['source_link'] = array(
			'#type' => 'textfield',
			'#title' => t('Source Link URL'),
			'#attributes' => array('class' => 'source_link')
		);
		
		$form['short_desc'] = array(
			'#type' => 'textarea',
			'#title' => t('Short Description'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'short_desc')
		);
		
		$form['permission_file'] = array(
			'#type' => 'file',
			'#title' => t('Permission File <span class="upload-help-test"> (Upload .pdf file only)</span>'),
			'#attributes' => array('class' => 'permission_file'),
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}

	function add_new_testimonial_form_validate($form, &$form_state){
		if($_FILES['files']['tmp_name']['permission_file'] == ''){
			form_set_error('permission_file', t('Please provide permission file.'));
		}
		else{
			$allowed_file_types = array('.pdf');
			$file_ext = substr($_FILES['files']['name']['permission_file'], strripos($_FILES['files']['name']['permission_file'], '.'));
			if (!in_array($file_ext,$allowed_file_types)) {
				form_set_error('permission_file', t('Invalid file format, please select .pdf file format.'));
			}else {
				return true;
			}
		}
	}
	
	function add_new_testimonial_form_submit($form, &$form_state) {
		global $user;
		$path = variable_get('testimonial_file_path', '').'/';
		$file_name = $user->uid."_".strtotime(date('Y-m-d H:i:s')).".pdf";
		if(move_uploaded_file($_FILES['files']['tmp_name']['permission_file'], $path.$file_name)){
			if($form_state['values']['modified_body'])
				$query = "insert into testimonials (original_body, modified_body, source, source_link_title, source_link, short_desc, permission_file, uploader_uid, uploaded_time) values ('".db_escape_string(trim($form_state['values']['original_body']))."', '".db_escape_string(trim($form_state['values']['modified_body']))."', '".db_escape_string(trim($form_state['values']['source']))."', '".db_escape_string(trim($form_state['values']['source_link_title']))."', '".db_escape_string(trim($form_state['values']['source_link']))."', '".db_escape_string(trim($form_state['values']['short_desc']))."', '".$file_name."', ".$user->uid.", '".date('Y-m-d H:i:s')."')";
			else
				$query = "insert into testimonials (original_body, source, source_link_title, source_link, short_desc, permission_file, uploader_uid, uploaded_time) values ('".db_escape_string(trim($form_state['values']['original_body']))."', '".db_escape_string(trim($form_state['values']['source']))."', '".db_escape_string(trim($form_state['values']['source_link_title']))."', '".db_escape_string(trim($form_state['values']['source_link']))."', '".db_escape_string(trim($form_state['values']['short_desc']))."', '".$file_name."', ".$user->uid.", '".date('Y-m-d H:i:s')."')";
			if(db_query($query)) {
				drupal_set_message('Testimonial added successfully!');
			}else {
				drupal_set_message('Something went wrong, please try again later.');
			}
		}else {
			drupal_set_message('# Something went wrong, please try again later.');
		}
	}
	
	function edit_testimonial_form($form_state, $id) {
		
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$id = base64_decode($id);
		if($id) {
			$query = "select * from testimonials where id=".$id;
			$result = db_query($query);
			$row = db_fetch_object($result);

			if($row) {
				$form['original_body'] = array(
					'#type' => 'textarea',
					'#title' => t('Original Content'),
					'#required' => TRUE,
					'#attributes' => array('class' => 'original_body'),
					'#value' => $row->original_body,
				);
				$form['modified_body'] = array(
					'#type' => 'textarea',
					'#title' => t('Modified Content'),
					'#required' => TRUE,
					'#attributes' => array('class' => 'modified_body'),
					'#value' => $row->modified_body,
				);
				$form['source'] = array(
					'#type' => 'textfield',
					'#title' => t('Name (Source)'),
					'#required' => TRUE,
					'#attributes' => array('class' => 'source'),
					'#prefix' => $tmp_string,
					'#value' => $row->source,
				);
				$form['source_link_title'] = array(
					'#type' => 'textfield',
					'#title' => t('Source link title'),
					'#attributes' => array('class' => 'source_link_title'),
					'#value' => $row->source_link_title,
				);
				$form['source_link'] = array(
					'#type' => 'textfield',
					'#title' => t('Source link'),
					'#attributes' => array('class' => 'source_link'),
					'#value' => $row->source_link,
				);
				$form['short_desc'] = array(
					'#type' => 'textarea',
					'#title' => t('Short description'),
					'#required' => TRUE,
					'#attributes' => array('class' => 'short_desc'),
					'#value' => $row->short_desc,
				);
				$form['permission_file'] = array(
					'#type' => 'item',
					'#title' => 'Permission file',
					'#value' => t(l($row->permission_file, variable_get('testimonial_url_path', '').'/'.$row->permission_file)),
				);
				$form['uploader_uid'] = array(
					'#type' => 'item',
					'#title' => 'Uploaded by',
					'#value' => testimonials_get_user_info($row->uploader_uid),
				);
				$form['uploaded_time'] = array(
					'#type' => 'item',
					'#title' => 'Uploaded time',
					'#value' => t($row->uploaded_time),
				);
				$form['id'] = array(
					'#type' => 'hidden',
					'#required' => true,
					'#value' => $id,
				);
				$form['submit'] = array(
					'#type' => 'submit',
					'#value' => t('Update')
				);
				
			}else {
				$form['warning'] = array(
					'#type' => 'item',
					'#value' => t('Something went wrong, please contact site administrator for more details.'),
				);
			}
		}else {
			$form['warning'] = array(
				'#type' => 'item',
				'#value' => t('Access Denied!'),
			);
		}
		
		return $form;
	}
	
	function edit_testimonial_form_submit($form, &$form_state) {
	
		global $user;
	
		$query = "update testimonials set original_body='".db_escape_string(trim($_POST['original_body']))."', modified_body='".db_escape_string(trim($_POST['modified_body']))."', source='".db_escape_string(trim($_POST['source']))."', source_link_title='".db_escape_string(trim($_POST['source_link_title']))."', source_link='".db_escape_string(trim($_POST['source_link']))."', short_desc='".db_escape_string(trim($_POST['short_desc']))."', publisher_uid='".$user->uid."', published_time='".date('Y-m-d H:i:s')."' where id=".$_POST['id'];
		if(db_query($query)) {
			drupal_set_message("Testimonial updated successfully!");
		}else {
			drupal_set_message("Something went wrong, please try again later.");
		}
		drupal_goto('testimonial/list/uploaded', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}
	
	function list_uploaded_testimonials() {
		global $user;
		
		$query = "select * from testimonials where uploader_uid=".$user->uid;
		$count_query = "select count(*) from organiser where uploader_uid=".$user->uid;
		
		$result = pager_query($query, 5, 0, $count_query);
		$output = '';
		$row = array();
		$sno = 1;
		while ($item = db_fetch_object($result)) {
			$tmp_string = '<div style="padding: 5px;"><label class="form-item"><b>Content:</b></label><br><div style="padding-left: 15px;">'.testimonial_get_body($item->original_body);
			if($item->modified_body)
				$tmp_string .= '<label class="form-item"><b>Modified Content:</b></label><br><div style="padding-left: 15px;">'.testimonial_get_body($item->modified_body);
			if($item->source_link)
				$tmp_string .= '<p><a href="'.$item->source_link.'">'.(($item->source_link_title != '')?$item->source_link_title:$item->source_link).'</a></p>';
			$tmp_string .= '<p>Permission file: '.l($item->permission_file, variable_get('testimonial_url_path', '').'/'.$item->permission_file).'</p>';
			$tmp_string .= '</div>';
			$tmp_string .= '<p style="text-align: right; font-weight: bold; font-size: 13px;">- '.$item->source.'</p>'.'<div style="float: left">'.testimonials_get_user_info($item->uploader_uid).' &nbsp'.$item->uploaded_time.'</div>';
			if($item->status == 0)
				$tmp_string .= '<div style="float: right">'.l('Edit', 'testimonial/edit/'.base64_encode($item->id)).' &nbsp;|&nbsp; '.l('Delete', 'testimonial/delete/'.base64_encode($item->id)).'</div>';
			$tmp_string .='</div></div>';
			$row[] = array($tmp_string);
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output .= theme_table(null,$row);
			$output .= theme_pager(null, 5);
		}
		
		return $output;
	}
	
	function delete_testimonial($id = NULL) {
		$id = base64_decode($id);
		if($id) {
			$query = "delete from testimonials where id=".$id;
			db_query($query);
			drupal_set_message('Testimonial deleted!');
		}else {
			drupal_set_message('Access Denied!');
		}
		drupal_goto('testimonial/list/uploaded', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}
?>
