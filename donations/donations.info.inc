<?php

/**
 * Implements HOOK_entity_property_info
 * 
 * This is used to integrate our entities with the Views module.
 * This requires the Entity API (entity) module.
 */
function donations_entity_property_info()
{

	$info = array();
	$info['funds']['properties'] = _entity_property_info_funds();
	$info['donors']['properties'] = _entity_property_info_donors();
	$info['donations']['properties'] = _entity_property_info_donations();
	return $info;
}

function _entity_property_info_funds()
{
	return array(
		'id' => array(
			'description' => t('The primary identifier for a funds entry.'),
			'type' => 'integer',
			'label' => t('ID'),
			'schema field' => 'id',
		),
		'fund_name' => array(
			'description' => t('The name of the fund.'),
			'type' => 'text',
			'label' => t('Name'),
			'schema field' => 'fund_name',
		),
	);
}

function _entity_property_info_donors()
{
	return array(
		'id' => array(
			'description' => t('The primary identifier for a donor.'),
			'type' => 'integer',
			'label' => t('ID'),
			'schema field' => 'id',
		),
		'name' => array(
			'description' => t('The name of the donor.'),
			'type' => 'text',
			'label' => t('Name'),
			'schema field' => 'name',
		),
		'sort_name' => array(
			'description' => t('The name of the donor that can be sorted.'),
			'type' => 'text',
			'label' => t('Sort Name'),
			'schema field' => 'sort_name',
		),
					// Add more fields as required to expose within Views.
	);
}

function _entity_property_info_donations()
{
	return array(
		'id' => array(
			'description' => t('The primary identifier for a donation.'),
			'type' => 'integer',
			'label' => t('ID'),
			'schema field' => 'id',
		),
		'donor_id' => array(
			'description' => t('Identifies the donor.'),
			'type' => 'donors',             // <== This lets us define a relationship to the "donors" entity (join to the 'donors' table).
			'label' => t('Donor ID'),
			'schema field' => 'donor_id',
		),
		'amount' => array(
			'description' => t('The amount of the donation.'),
			'type' => 'integer',
			'label' => t('Amount'),
			'schema field' => 'amount',
		),
		'fund_id' => array(
			'description' => t('The Fund for the donation.'),
			'type' => 'funds',            // <== This lets us define a relationship to the "funds" entity (join to the 'funds' table).
			'label' => t('Fund'),
			'schema field' => 'fund_id',
		),
		'approach_id' => array(
			'description' => t('The Approach for the donation.'),
			'type' => 'integer',
			'label' => t('Approach'),
			'schema field' => 'approach_id',
		),
		'date_donated' => array(
			'description' => t('The date of the donation.'),
			'type' => 'date',
			'label' => t('Date'),
			'schema field' => 'date_donated',
		),
		'note' => array(
			'description' => t('The Note for the donation.'),
			'type' => 'text',
			'label' => t('Note'),
			'schema field' => 'note',
		),
		'entered_by' => array(
			'description' => t('The uis of the user who entered the donation.'),
			'type' => 'integer',
			'label' => t('Entered By'),
			'schema field' => 'entered_by',
		),
	);
}

