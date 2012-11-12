<?php

/**
 * This is the form for adding a donation from a donor who is identified by the $p_id.
 * 
 * @param array $form
 * @param array $form_state
 * @param int $p_id
 * @return array
 */
function form_add_donation($form, $form_state, $p_id)
{
  // Retreive the donor information using the Entity API.
	$donor = entity_load_single('donors', $p_id);
  
  // Build the form.
	$form = array();

	$form['donor_name'] = array(
		'#type' => 'markup',
		'#markup' => '<p>Adding a donation from <strong>' . $donor->name . '</strong></p>',
	);

	$form['donor_id'] = array(
		'#type' => 'value',
		'#value' => $p_id,
	);

	$form['date_donated'] = array(
		'#type' => 'date_popup',
		'#date_format' => 'm-d-Y',
    '#default_value' => date('Y-m-d H:i:s', REQUEST_TIME),
		'#required' => true,
    '#attributes' => array(
      'onmouseover' => "this.style.cursor='pointer'",
      'onmouseout' => "this.style.cursor='default'",
    ),
  );
  
	$form['amount'] = array(
		'#type' => 'textfield',
    '#size' => 6,
    '#default_value' => '',
		'#required' => true,
    '#field_prefix' => '$',
		'#title' => t('Amount'),
    '#rules' => array('numeric'),
	);

	$form['fund_id'] = array(
		'#type' => 'select',
		'#options' => _get_funds_options(),
		'#title' => t('Fund name'),
	);

	$form['note'] = array(
		'#type' => 'textarea',
		'#title' => t('Notes'),
	);

	$form['actions'] = array(
		'add' => array(
			'#type' => 'submit',
			'#value' => 'save',
		),
		'cancel' => array(
			'#type' => 'submit',
			'#value' => 'cancel',
			'#submit' => array('form_add_donation_cancel'),
      '#limit_validation_errors' => array(),
		),
	);

	return $form;
}

/**
 * This is called the add donation form is submitted.
 * 
 * @global object $user
 * @param array $form
 * @param array $form_state
 */
function form_add_donation_submit($form, &$form_state)
{
	global $user;

	$donation = $form_state['values'];

  // We probably do not have to unset anything because the database abstraction layer
  // is clever enough to only store relevanty fields.
	unset($donation['form_build_id']);
	unset($donation['form_token']);
	unset($donation['form_id']);
	unset($donation['op']);
	unset($donation['save']);
	unset($donation['cancel']);
	
  // Set some values we always want to store in the entity.
  $donation['date_donated'] = strtotime($donation['date_donated']);
  $donation['date_entered'] = REQUEST_TIME;
	$donation['entered_by'] = $user->uid;

  // Use the Entity API to insert the new donation record.
  if (entity_save('donations', (object) $donation) == SAVED_NEW)
	{
		drupal_set_message('The new Donation has been saved.');
	}

	$form_state['redirect'] = '/donations/' .$form['donor_id'];
}

/**
 * This is called when the user cancels the add donation form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_add_donation_cancel($form, &$form_state)
{
	$form_state['redirect'] = 'donations/' .$form['donor_id']['#value'];
}

/**
 * This is called to edit a donation.  It builds and returns a form.
 * 
 * @param array $form
 * @param array $form_state
 * @param int $p_id
 * @return array
 */
function form_edit_donation($form, &$form_state, $p_id)
{
  // Use the Entity API to retrieve the donation uniquely identified by $p_id.
 	$donation = entity_load_single('donations', $p_id);
  
  // Use the Entity API to retrieve the donor who made the donation.
  $donor = entity_load_single('donors', $donation->donor_id);

  $form = array();

	$form['donor_name'] = array(
		'#type' => 'markup',
		'#markup' => '<p>Editing a donation from <strong>' . $donor->name . '</strong></p>',
	);
  
	$form['id'] = array(
		'#type' => 'value',
		'#value' => $p_id,
	);
  
	$form['date_donated'] = array(
		'#type' => 'date_popup',
		'#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d', $donation->date_donated),
	);
  
	$form['amount'] = array(
		'#type' => 'textfield',
    '#size' => 6,
    '#default_value' => $donation->amount,
		'#required' => true,
    '#field_prefix' => '$',
		'#title' => t('Amount'),
    '#rules' => array('numeric'),
	);

	$form['fund_id'] = array(
		'#type' => 'select',
		'#options' => _get_funds_options(),
		'#title' => t('Fund name'),
    '#default_value' => $donation->fund_id,
	);

	$form['note'] = array(
		'#type' => 'textarea',
		'#title' => t('Notes'),
    '#default_value' => $donation->note,
	);

	$form['actions'] = array(
		'add' => array(
			'#type' => 'submit',
			'#value' => 'save',
		),
		'cancel' => array(
			'#type' => 'submit',
			'#value' => 'cancel',
			'#submit' => array('form_edit_donation_cancel'),
      '#limit_validation_errors' => array(),
		),
	);
  
  return $form;
}

/**
 * This is called when the edit donation form is submitted.
 * 
 * @global object $user
 * @param type $form
 * @param array $form_state
 */
function form_edit_donation_submit($form, &$form_state)
{
 	global $user;

	$donation = $form_state['values'];

  // We probably do not have to unset anything because the database abstraction layer
  // is clever enough to only store relevanty fields.
	unset($donation['form_build_id']);
	unset($donation['form_token']);
	unset($donation['form_id']);
	unset($donation['op']);
	unset($donation['save']);
	unset($donation['cancel']);
	
  // Set some values we always want to store in the entity.
  $donation['date_donated'] = strtotime($donation['date_donated']);
  $donation['date_modified'] = REQUEST_TIME;
	$donation['modified_by'] = $user->uid;
  
  // Use the Entity API to update the donation.
  if (entity_save('donations', (object) $donation) == SAVED_UPDATED)
	{
		drupal_set_message('The Donation has been updated.');
	}

	$form_state['redirect'] = '/donors'; 
}

/**
 * This is called when the user cancels the edit donation form.
 * @param array $form
 * @param array $form_state
 */
function form_edit_donation_cancel($form, &$form_state)
{
	$form_state['redirect'] = 'donors';
}

/**
 * This is called to delete a donation uniquely identified by $p_id.
 * 
 * @param array $form
 * @param array $form_state
 * @param int $p_id
 * @return array
 */
function form_delete_donation($form, $form_state, $p_id)
{
  $form = array();
  
  $form['donation_id'] = array(
    '#type' => 'value',
    '#value' => $p_id,
  );
  
  $form['alert'] = array(
      '#type' => 'markup',
      '#markup' => '<p class="warning">Are you sure you want to delete this donation?  This action cannot be undone.</p>',
  );
  
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => 'Delete',
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => 'Cancel',
      '#submit' => array('form_delete_donation_cancel'),
      '#limit_validation_errors' => array(),
    ),
  );
  
  return $form;
}

/**
 * This is called when the delete dontation form is submitted.
 * @param array $form
 * @param array $form_state
 */
function form_delete_donation_submit($form, &$form_state)
{
  // Use the Entity API to delete the donation identified by the 'donation_id'.
  entity_delete('donations', $form_state['values']['donation_id']);
  
  drupal_set_message('The donation has been deleted.');
 	$form_state['redirect'] = 'donors';  
}

/**
 * This is called when the user cancels the delete donation form.
 * @param array $form
 * @param array $form_state
 */
function form_delete_donation_cancel($form, &$form_state)
{
  	$form_state['redirect'] = 'donors';
}