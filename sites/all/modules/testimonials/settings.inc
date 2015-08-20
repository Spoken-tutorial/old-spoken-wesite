<?php
	function testimonial_settings_form($form_state){
		$form['testimonial_file_path'] = array(
			'#type' => 'textfield',
			'#title' => t('Permission file path'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('testimonial_file_path', ''),
		);
		$form['testimonial_url_path'] = array(
			'#type' => 'textfield',
			'#title' => t('Permission file URL path'),
			'#size' => 50,
			'#required' => TRUE,
			'#default_value' => variable_get('testimonial_url_path', ''),
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	
	function testimonial_settings_form_submit($form, &$form_state){
		variable_set('testimonial_file_path', $_POST['testimonial_file_path']);
		variable_set('testimonial_url_path', $_POST['testimonial_url_path']);
		drupal_set_message(t('Settings updated'), 'status');
	}
