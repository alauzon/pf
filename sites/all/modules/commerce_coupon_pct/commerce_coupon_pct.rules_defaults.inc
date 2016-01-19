<?php

/**
 * @file
 * Percentage coupon default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_coupon_pct_default_rules_configuration() {
  $rules = array();

  $rule = rules_reaction_rule(array(), array(
    'commerce_coupon_line_item' => array(
      'type' => 'commerce_coupon_line_item',
      'label' => 'commerce coupon line item')
      )
  );

  $rule = rules_reaction_rule();
  $rule->label = t('Calculate coupon with percentage amount');
  $rule->active = TRUE;

  $rule
      ->event('commerce_coupon_redeem')
      ->condition('data_is', array('data:select' => 'coupon:type', 'op' => '=', 'value' => 'commerce_coupon_pct'))
      ->condition('entity_has_field', array('entity:select' => 'coupon', 'field' => 'commerce_coupon_percent_amount'))
      ->condition(rules_and()->condition('data_is_empty', array('data:select' => 'coupon:commerce-coupon-percent-amount'))->negate())
      ->condition('data_is', array('data:select' => 'coupon:commerce-coupon-percent-amount', 'op' => '>', 'value' => 1))
      ->condition('data_is', array('data:select' => 'coupon:is-active', 'op' => '=', 'value' => TRUE))
      ->action('commerce_coupon_action_set_granted_amount_by_percentage', array(
        'commerce_order:select' => 'commerce-order',
        'commerce_coupon_log:select' => 'coupon-log',
        'rate:select' => 'coupon:commerce-coupon-percent-amount',
        'round_mode' => '1'
      ));

  $rules['commerce_coupon_pct_percent_amount'] = $rule;

  return $rules;
}
