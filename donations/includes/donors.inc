<?php
/**
 * Builds the main edit donor form.
 * 
 * @param array $form_state
 * @return array
 */
function form_maintain_donors($form_state)
{
  $form = array();

  $form['table'] = array(
    '#type' => 'markup',
    '#markup' => _build_donor_table(),  // Use a helper function to build a table of all donors.
  );

  $form['actions'] = array(
    'done' => array(
      '#type' => 'submit',
      '#value' => 'Done',
    ),
    'add' => array(
      '#type' => 'submit',
      '#value' => 'Add new...',
    ),
  );

  return $form;
}

/**
 * This is called when the maintain donors form is submitted.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_maintain_donors_submit($form, &$form_state)
{
  switch ($form_state['values']['op'])
  {
    case 'Done':
      $form_state['redirect'] = '<front>';
      break;

    case 'Add new...':
      $form_state['redirect'] = '/donors/add';
      break;
  }
}

/**
 * This builds the add donor form.
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function form_add_donor($form, $form_state)
{
  $form = _build_donor_form();
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => 'Save',
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => 'Cancel',
      '#submit' => array('form_add_donor_cancel'),
      '#limit_validation_errors' => array(),
    ),
  );
	return $form;
}

/**
 * This is called when the user cancels the add donor form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_add_donor_cancel($form, &$form_state)
{
  $form_state['redirect'] = '/donors';
}

/**
 * This is called when the user submits the add donor form.
 * @global object $user
 * @param array $form
 * @param array $form_state
 */
function form_add_donor_submit($form, &$form_state)
{
  global $user;
  $values = &$form_state['values'];

  if ($values['op'] == 'Save')
  {
    $donor = array();
    foreach ($values as $key => $value)
    {
      $donor[$key] = $value;
    }
    
    // Set some values we always want to store in the entity.
    $donor['date_created'] = REQUEST_TIME;
    $donor['created_by'] = $user->uid;
    
    // We probably do not have to unset anything because the database abstraction layer
    // is clever enough to only store relevanty fields.
    unset($donor['form_build_id']);
    unset($donor['form_token']);
    unset($donor['form_id']);
    unset($donor['op']);
    unset($donor['save']);
    unset($donor['cancel']);

    // Use the Entity API to create and save a new donor record.
    $new_donor = entity_create('donors', $donor);
    if (entity_save('donors', $new_donor) == SAVED_NEW)
    {
      drupal_set_message('The new Donor has been saved.');
    }
  }
  
  $form_state['redirect'] = 'donations/'. $new_donor->id .'/add';
}

/**
 * This builds the edit donor form.
 * 
 * @param array $form
 * @param array $form_state
 * @param int $p_id
 * @return array
 */
function form_edit_donor($form, $form_state, $p_id)
{
  // Use the Entity API to load the donor that is uniquely identified by $p_id.
  $donors = entity_load('donors', array($p_id));
  
  // This gets the first (and hopefully only) donor.
  $donor = reset($donors);

  $form = array();
	
	$form['links'] = array(
		'#type' => 'markup',
		'#markup' => l('View donations', 'donations/' . $p_id) .'&nbsp;&nbsp;&nbsp;&nbsp;' .l('Add donation', 'donations/' . $p_id .'/add'),
	);
	
  // Attach the main body of the donor form that is built with a helper function.
	$form += _build_donor_form((array) $donor);
  
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $p_id,
  );
  
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => 'Save',
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => 'Cancel',
      '#submit' => array('form_edit_donor_cancel'),
      '#limit_validation_errors' => array(),
    ),
  );

  return $form;
}

/**
 * This is called when the user cancels the edit donor form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_edit_donor_cancel($form, &$form_state)
{
  $form_state['redirect'] = '/donors';
}

/**
 * This is called when the user submits the edit donor form.
 * 
 * @global object $user
 * @param array $form
 * @param array $form_state
 */
function form_edit_donor_submit($form, &$form_state)
{
  global $user;
  $values = &$form_state['values'];

  if ($values['op'] == 'Save')
  {
    $donor = array();
    foreach ($values as $key => $value)
    {
      $donor[$key] = $value;
    }

    // Set some values we always want to store in the entity.
    $donor['date_modified'] = REQUEST_TIME;
    $donor['modified_by'] = $user->uid;

    // We probably do not have to unset anything because the database abstraction layer
    // is clever enough to only store relevanty fields.
    unset($donor['form_build_id']);
    unset($donor['form_token']);
    unset($donor['form_id']);
    unset($donor['op']);
    unset($donor['save']);
    unset($donor['cancel']);

    // Use the Entity API to update the donor record.
    if (entity_save('donors', (object) $donor) == SAVED_UPDATED)
    {
      drupal_set_message('The Donor has been updated.');
    }
  }
  $form_state['redirect'] = '/donors';
}

/**
 * This build a form that is used to delete a donor that is uniquely identified by $p_pid.
 * @param array $form
 * @param array $form_state
 * @param int $p_id
 * @return array
 */
function form_delete_donor($form, &$form_state, $p_id)
{
  // Use the Entity API to get the donors identified by $p_id.  There should only be one.
  $donors = entity_load('donors', array($p_id));
  
  // Get the first (and hopefluly only) donor.
  $donor = reset($donors);

  $form = array();
  
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $p_id,
  );
  
  $form['name'] = array(
      '#type' => 'markup',
      '#markup' => '<p class="warning">Are you sure you want to delete ' .$donor->name .' and all of their donations?  This action cannot be undone.</p>',
  );
  
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => 'Delete',
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => 'Cancel',
      '#submit' => array('form_delete_donor_cancel'),
      '#limit_validation_errors' => array(),
    ),
  );
  
  return $form;
}

/**
 * This is called then the user submits the delete donor form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_delete_donor_submit($form, &$form_state)
{
  // Delete all the donations from this domor.
  db_delete('donations')
    ->condition('donor_id', $form_state['values']['id'])
    ->execute();
  
  // Use the Entity API to delete the donor.
  entity_delete('donors', $form_state['values']['id']);
  
  drupal_set_message('The donor has been deleted.' );
  $form_state['redirect'] = 'donors';
}

/**
 * This is called when the user cancels the delete donor form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_delete_donor_cancel($form, &$form_state)
{
  $form_state['redirect'] = 'donors';
}

/**
 * This is a helper function that returns a themed table of all donors.

 * @return array
 */
function _build_donor_table()
{
  $header = array('ID', 'Name', 'City', 'Country', 'Actions');

  // Use the Entity API to retrieve all donors.
  $donors = entity_load('donors');

  $rows = array();
  foreach ($donors as $donor)
  {
    $rows[] = array(
      $donor->id,
      $donor->name,
      $donor->city,
      $donor->country,
      '<a href="/donors/edit/' . $donor->id . '">Edit</a>',
    );
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Helper function to build the main part of the donor form.
 * 
 * @param array $vals
 * @return array
 */
function _build_donor_form($vals = null)
{
  return array(
    'name' => array(
      '#type' => 'textfield',
      '#title' => 'Donor Name',
      '#required' => true,
      '#default_value' => ($vals == null) ? '' : $vals['name'],
    ),
    'sort_name' => array(
      '#type' => 'textfield',
      '#title' => 'Sort Name',
      '#description' => 'The name to be used for sorting.',
      '#required' => true,
      '#default_value' => ($vals == null) ? '' : $vals['sort_name'],
    ),
    'address' => array(
      '#type' => 'textarea',
      '#rows' => 1,
			'#columns' => 10,
      '#title' => 'Address',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['address'],
    ),
    'city' => array(
      '#type' => 'textfield',
      '#title' => 'City',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['city'],
    ),
    'state' => array(
      '#type' => 'textfield',
      '#title' => 'State',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['state'],
    ),
    'country' => array(
      '#type' => 'select',
      '#title' => 'Country',
      '#default_value' => ($vals == null) ? 'US' : $vals['country'],
      '#options' => countries_get_countries('name', array('enabled' => COUNTRIES_ENABLED)),
    ),
    'postal_code' => array(
      '#type' => 'textfield',
      '#title' => 'Postal code',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['postal_code'],
    ),
    'email' => array(
      '#type' => 'textfield',
      '#title' => 'Email',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['email'],
      '#rules' => array('email'),
    ),
    'phone_primary' => array(
      '#type' => 'textfield',
      '#title' => 'Phone (primary)',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['phone_primary'],
    ),
    'phone_secondary' => array(
      '#type' => 'textfield',
      '#title' => 'Phone (secondary)',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['phone_secondary'],
    ),
    'notes' => array(
      '#type' => 'textarea',
      '#title' => 'Notes',
      '#required' => false,
      '#default_value' => ($vals == null) ? '' : $vals['notes'],
    ),
    'anonymous' => array(
      '#type' => 'checkbox',
      '#title' => 'Remain anonymous',
      '#default_value' => ($vals == null) ? '' : $vals['anonymous'],
    ),
    'receive_emails' => array(
      '#type' => 'checkbox',
      '#title' => 'Receive emails',
      '#default_value' => ($vals == null) ? '' : $vals['receive_emails'],
    ),
  );
}
