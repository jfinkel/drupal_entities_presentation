<?php

/**
 * This builds the maintain funcds form.
 * 
 * @param array $form_state
 * @return array
 */
function form_maintain_funds($form_state)
{
	$form = array();

	$form['table'] = array(
		'#type' => 'markup',
		'#markup' => _build_fund_table(),  // Use a helper function to create a table of all funds.
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
 * Returns a sorted array of all funds.
 * 
 * @return array
 */
function _get_funds_options()
{
  // Use the Entity API to retreive all funds.
	$funds = entity_load('funds');
  
	$answer = array();
	foreach ($funds as $fund_id => $fund)
	{
		$answer[$fund_id] = ucwords($fund->fund_name);
	}

  // Sort the array.
  asort($answer);
  
	return $answer;
}

/**
 * Return an HTML table thaty contains all funds.
 * 
 * @return string
 */
function _build_fund_table()
{
	$header = array('ID', 'Name', 'Actions');

  // Use the Entity API to retreive all funds.
	$funds = entity_load('funds');

	$rows = array();
	foreach ($funds as $fund)
	{
		$rows[] = array(
			$fund->id,
			$fund->fund_name,
			'<a href="/funds/edit/' . $fund->id . '">Edit</a>',
		);
	}

	$output = theme('table', array('header' => $header, 'rows' => $rows));
	return $output;
}

/**
 * This is called when the maintain funds form is submitted.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_maintain_funds_submit($form, &$form_state)
{
	switch ($form_state['values']['op'])
	{
		case 'Done':
			$form_state['redirect'] = '<front>';
			break;

		case 'Add new...':
			$form_state['redirect'] = 'funds/add';
			break;
	}
}

/**
 * Builds the add fund form.
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function form_add_fund($form, $form_state)
{
	return array(
		'name' => array(
			'#type' => 'textfield',
			'#label' => 'Fund Name',
			'#attributes' => array('placeholder' => 'Enter fund name'),
			'#required' => true,
		),
		'actions' => array(
			'save' => array(
				'#type' => 'submit',
				'#value' => 'Save',
			),
			'cancel' => array(
				'#type' => 'submit',
				'#value' => 'Cancel',
				'#submit' => array('form_add_fund_cancel'),
				'#limit_validation_errors' => array(),
			),
		),
	);
}

/**
 * This is called when the user cancels the add fund form.
 * @param array $form
 * @param array $form_state
 */
function form_add_fund_cancel($form, &$form_state)
{
	$form_state['redirect'] = 'funds';
}

/**
 * This is called when the user submits the add fund form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_add_fund_submit($form, &$form_state)
{
	$values = &$form_state['values'];

	if ($values['op'] == 'Save')
	{
		$fund = array(
			'fund_name' => $values['name'],
		);

    // Use the Entity API to store the new fund record.
		if (entity_save('funds', (object) $fund) == SAVED_NEW)
		{
			drupal_set_message('The new Fund has been saved.');
		}
	}
	$form_state['redirect'] = 'funds';
}

/**
 * This builds a form to edit the fund that is uniquely identified by $p_id.
 * 
 * @param array $form
 * @param array $form_state
 * @param int $p_id
 * @return type
 */
function form_edit_fund($form, $form_state, $p_id)
{
  // Use the Entity API to retrieve the funds with the specified id.
	$funds = entity_load('funds', array($p_id));
  
  // Get the first (and hopefully only) fund.
	$fund = reset($funds);

	return array(
		'name' => array(
			'#type' => 'textfield',
			'#default_value' => $fund->fund_name,
			'#required' => true,
		),
		'id' => array(
			'#type' => 'value',
			'#value' => $p_id,
		),
		'actions' => array(
			'save' => array(
				'#type' => 'submit',
				'#value' => 'Save',
			),
			'cancel' => array(
				'#type' => 'submit',
				'#value' => 'Cancel',
				'#submit' => array('form_edit_fund_cancel'),
				'#limit_validation_errors' => array(),
			),
		),
	);
}

/**
 * This is called when the user cancels the edit fund form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_edit_fund_cancel($form, &$form_state)
{
	$form_state['redirect'] = 'funds';
}

/**
 * This is called when the user submits the edit fund form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_edit_fund_submit($form, &$form_state)
{
	$values = &$form_state['values'];

	if ($values['op'] == 'Save')
	{
		$fund = array(
			'id' => $values['id'],
			'fund_name' => $values['name'],
		);

    // Use the Entity API to update the record.
		if (entity_save('funds', (object) $fund) == SAVED_UPDATED)
		{
			drupal_set_message('The Fund has been saved.');
		}
	}
	$form_state['redirect'] = 'funds';
}