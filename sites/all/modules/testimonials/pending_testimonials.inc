<?php
	function list_pending_testimonials() {
		
		$query = "select * from testimonials where status=0";
		$count_query = "select count(*) from organiser where status=0";
		
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
			$tmp_string .= '<p style="text-align: right; font-weight: bold;">- '.$item->source.'</p>'.'<div style="float: left">'.testimonials_get_user_info($item->uploader_uid).' &nbsp'.$item->uploaded_time.'</div><div style="float: right">'.l('Modify', 'testimonial/modify/'.base64_encode($item->id)).' &nbsp;|&nbsp; '.l('Publish', 'testimonial/publish/'.base64_encode($item->id)).' &nbsp;|&nbsp; '.l('Reject', 'testimonial/reject/'.base64_encode($item->id)).'</div></div></div>';
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
	
	function modify_testimonial_form($form_state, $id) {
		
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$id = base64_decode($id);
		if($id) {
			$query = "select * from testimonials where id=".$id;
			$result = db_query($query);
			$row = db_fetch_object($result);

			if($row) {
				$tmp_string = '';
				if($row->modified_body) {
					$tmp_string = '<div class="form-item" id="edit-modified-body-wrapper">
 <label for="edit-modified-body">Content: <span class="form-required" title="This field is required.">*</span></label>
 <div class="resizable-textarea"><span><textarea cols="60" rows="15" name="modified_body" id="edit-modified-body" class="form-textarea resizable required modified_body textarea-processed">'.$row->modified_body.'</textarea><div class="grippie" style="margin-right: -447px;"></div></span></div></div>';
				}else {
					$tmp_string = '<div class="form-item" id="edit-modified-body-wrapper">
 <label for="edit-modified-body">Content: <span class="form-required" title="This field is required.">*</span></label>
 <div class="resizable-textarea"><span><textarea cols="60" rows="15" name="modified_body" id="edit-modified-body" class="form-textarea resizable required modified_body textarea-processed">'.$row->original_body.'</textarea><div class="grippie" style="margin-right: -447px;"></div></span></div></div>';
				}
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
					'#value' => t('Publish')
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
	
	function modify_testimonial_form_submit($form, &$form_state) {
	
		global $user;
	
		$query = "update testimonials set modified_body='".db_escape_string(trim($_POST['modified_body']))."', source='".db_escape_string(trim($_POST['source']))."', source_link_title='".db_escape_string(trim($_POST['source_link_title']))."', source_link='".db_escape_string(trim($_POST['source_link']))."', short_desc='".db_escape_string(trim($_POST['short_desc']))."', publisher_uid='".$user->uid."', published_time='".date('Y-m-d H:i:s')."', status='1' where id=".$_POST['id'];
		if(db_query($query)) {
			drupal_set_message("Testimonial updated successfully!");
		}else {
			drupal_set_message("Something went wrong, please try again later.");
		}
		drupal_goto('testimonial/list/pending', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}
	
	function publish_testimonial($id = NULL) {
		$id = base64_decode($id);
		if($id) {
			$query = "update testimonials set status='1' where id=".$id;
			if(db_query($query)) {
				drupal_set_message('Testimonial published!');
			}else {
				drupal_set_message('Something went wrong, please try again later.');
			}
		}else {
			drupal_set_message('Access Denied!');
		}
		drupal_goto('testimonial/list/pending', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}
	
	function reject_testimonial($id = NULL) {
		$id = base64_decode($id);
		if($id) {
			$query = "update testimonials set status='2' where id=".$id;
			if(db_query($query)) {
				drupal_set_message('Testimonial rejected!');
			}else {
				drupal_set_message('Something went wrong, please try again later.');
			}
		}else {
			drupal_set_message('Access Denied!');
		}
		drupal_goto('testimonial/list/pending', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}
?>
