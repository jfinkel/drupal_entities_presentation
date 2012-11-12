<?php

/**
 * This returns a form that is used to generate reports.
 * 
 * @return array
 */
function form_report_main()
{
	$form = array(
		'date_range' => array(
			'#title' => 'Date Range',
			'#type' => 'select',
			'#options' => array(
				'custom' => 'Custom dates',
				'last year' => 'Last year',
				'last 2 years' => 'Last 2 years',
			),
			'#attributes' => array('onchange' => 'SetDates(this.value);'),
		),
		'start_date' => array(
			'#title' => 'Start Date',
			'#type' => 'date_popup',
			'#date_format' => 'Y-m-d',
			'#required' => true,
			'#attributes' => array(
				'onmouseover' => "this.style.cursor='pointer'",
				'onmouseout' => "this.style.cursor='default'",
			),
		),
		'end_date' => array(
			'#title' => 'Start Date',
			'#type' => 'date_popup',
			'#date_format' => 'Y-m-d',
			'#required' => true,
			'#attributes' => array(
				'onmouseover' => "this.style.cursor='pointer'",
				'onmouseout' => "this.style.cursor='default'",
			),
		),
		'floor_amount' => array(
			'#title' => 'Amount',
			'#description' => 'The minimum total amount per donor.',
			'#type' => 'textfield',
			'#field_prefix' => '$',
			'#required' => true,
			'#size' => 5,
			'#rules' => array('numeric'),
		),
		'cumulative' => array(
			'#type' => 'radios',
			'#title' => 'Range for totals',
			'#options' => array(
				'1' => 'Total over entire range',
				'0' => 'Total per year',
			),
			'#default_value' => 1,
		),
		'dates' => array(
			'last_year_start' => array(
				'#type' => 'hidden',
				'#value' => date('Y') - 1 . '-01-01',
				'#attributes' => array('id' => 'last_year_start'),
			),
			'last_year_end' => array(
				'#type' => 'hidden',
				'#value' => date('Y') - 1 . '-12-31',
				'#attributes' => array('id' => 'last_year_end'),
			),
			'two_years_ago_start' => array(
				'#type' => 'hidden',
				'#value' => date('Y') - 2 . '-01-01',
				'#attributes' => array('id' => 'two_years_ago_start'),
			),
			'two_years_ago_end' => array(
				'#type' => 'hidden',
				'#value' => date('Y') - 2 . '-12-31',
				'#attributes' => array('id' => 'two_years_ago_end'),
			),
		),
		'actions' => array(
			'submit' => array(
				'#type' => 'submit',
				'#value' => 'save',
			),
//			'cancel' => array(
//				'#type' => 'submit',
//				'#value' => 'cancel',
//				'#submit' => array('form_add_donation_cancel'),
//				'#limit_validation_errors' => array(),
//			),
		),
	);

	return $form;
}

/**
 * This is called when the user submits the report main form.
 * 
 * @param array $form
 * @param array $form_state
 */
function form_report_main_submit($form, &$form_state)
{
	$values = &$form_state['values'];
	$cumulative = $values['cumulative'];

	switch ($values['date_range'])
	{
		case 'custom':
			$start = $values['start_date'];
			$end = $values['end_date'];
			break;

		case 'last year':
			$year = date('Y') - 1;
			$start = $year . '-01-01';
			$end = $year . '-12-31';
			break;

		case 'last 2 years':
			$year1 = date('Y') - 2;
			$year2 = date('Y') - 1;
			$start = $year1 . '-01-01';
			$end = $year2 . '-12-31';
			break;
	}
	$floor = $values['floor_amount'];
	$form_state['redirect'] = "report/view/$start/$end/$floor/$cumulative";
}

/**
 * This returns an SQL query.
 * 
 * @param string $start_date
 * @param string $end_date
 * @param string $total_floor
 * @param bool $cumulative
 * @return string
 */
function _build_donations_report_query($start_date, $end_date, $total_floor, $cumulative = true)
{
	if ($cumulative)
	{
		return <<<EOD
SELECT 
  donors.name,
  SUM(donations.amount) AS total
FROM
  donations
  INNER JOIN donors ON (donations.donor_id = donors.id)
WHERE
  donations.date_donated BETWEEN UNIX_TIMESTAMP('$start_date') AND UNIX_TIMESTAMP('$end_date')
GROUP BY
  donors.name
HAVING
  SUM(donations.amount) >= $total_floor
ORDER BY
  donors.name
EOD;
	}
	else
	{
		$start1 = $start_date;
		$end1 = date("Y-m-d", strtotime(date("Y-m-d", strtotime($end_date)) . " - 1 year"));
		$start2 = date("Y-m-d", strtotime(date("Y-m-d", strtotime($start_date)) . " + 1 year"));
		$end2 = $end_date;
		return <<<EOD
(SELECT 
  donors.name,
  SUM(donations.amount) AS total,
  '$start1' as year
FROM
  donations
  INNER JOIN donors ON (donations.donor_id = donors.id)
WHERE
  donations.date_donated BETWEEN UNIX_TIMESTAMP('$start1') AND UNIX_TIMESTAMP('$end1')
GROUP BY
  donors.name
HAVING
  SUM(donations.amount) >= $total_floor
ORDER BY
  donors.name)
UNION
(SELECT 
  donors.name,
  SUM(donations.amount) AS total,
  '$start2' as year
FROM
  donations
  INNER JOIN donors ON (donations.donor_id = donors.id)
WHERE
  donations.date_donated BETWEEN UNIX_TIMESTAMP('$start2') AND UNIX_TIMESTAMP('$end2')
GROUP BY
  donors.name
HAVING
  SUM(donations.amount) >= $total_floor
ORDER BY
  donors.name)
EOD;
	}
}

