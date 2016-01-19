<?php

/**
 * @file
 * Coupon pct rules integration file.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_coupon_pct_rules_action_info() {
  $actions = array();

  $actions['commerce_coupon_action_set_granted_amount_by_percentage'] = array(
    'label' => t('Redeem a percentage coupon'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce Order'),
      ),
      'commerce_coupon_log' => array(
        'type' => 'commerce_coupon_log',
        'label' => t('Commerce Coupon Log'),
      ),
      'rate' => array(
        'type' => 'decimal',
        'label' => t('Percentage rate'),
      ),
      'round_mode' => array(
        'type' => 'integer',
        'label' => t('Price rounding mode'),
        'description' => t('Round the resulting price amount after performing this operation.'),
        'options list' => 'commerce_round_mode_options_list',
        'default value' => COMMERCE_ROUND_HALF_UP,
      ),
    ),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_set_granted_amount_by_percentage',
  );

  return $actions;
}

/**
 * Set the commerce coupon log to the right percentage amount.
 */
function commerce_coupon_action_set_granted_amount_by_percentage($order, $commerce_coupon_log, $rate, $round_mode) {
  if (!($commerce_coupon_log instanceof EntityMetadataWrapper)) {
    $commerce_coupon_log = entity_metadata_wrapper('commerce_coupon_log', $commerce_coupon_log);
  }
  $coupon = $commerce_coupon_log->commerce_coupon->value();
  $coupon_wrapper = entity_metadata_wrapper('commerce_coupon', $coupon);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  if ($rate > 1) {
    // Ensure that the rate is never bigger then 100%
    $rate = $rate / 100;
  }
  else {
    return;
  }

  // Calculate the total granted amount for this order.
  $amount = 0;
  // Loop over every line item on the order...
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    if ($line_item_wrapper->value()) {

      // Calculate the total granted amount for this line item
      $unit_price = commerce_price_wrapper_value($line_item_wrapper, 'commerce_unit_price', TRUE);
      $amount += commerce_round($round_mode, $unit_price['amount'] * $rate * $line_item_wrapper->quantity->value());
    }
  }
  // Get the price component to use in this price.
  $price_component_name = 'commerce_coupon_' . $coupon->type;
  drupal_alter('commerce_coupon_price_component_name', $price_component_name, $coupon);

  // Set the unit price on the coupon log object.
  $commerce_coupon_log->commerce_granted_amount->amount = $amount;
  $commerce_coupon_log->commerce_granted_amount->currency_code = $unit_price['currency_code'];

  // Add the granted amount to the components array.
  if (!commerce_price_component_load($commerce_coupon_log->commerce_granted_amount->value(), $price_component_name)) {
    $commerce_coupon_log->commerce_granted_amount->data = commerce_price_component_add(
        $commerce_coupon_log->commerce_granted_amount->value(), $price_component_name, $commerce_coupon_log->commerce_granted_amount->value(), TRUE, FALSE
    );
  }

  commerce_coupon_log_save($commerce_coupon_log->value());
}
