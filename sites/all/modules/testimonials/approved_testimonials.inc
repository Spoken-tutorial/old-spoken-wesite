<?php
	function list_approved_testimonials() {
		
		$query = "select * from testimonials where status=1";
		$count_query = "select count(*) from organiser where status=1";
		
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
			$tmp_string .= '<p style="text-align: right; font-weight: bold; font-size: 13px;">- '.$item->source.'</p>'.'<div style="float: left">'.testimonials_get_user_info($item->uploader_uid).' &nbsp'.$item->uploaded_time.'</div><div style="float: right">'.l('Modify', 'testimonial/modify/'.base64_encode($item->id)).'</div></div></div>';
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
?>
