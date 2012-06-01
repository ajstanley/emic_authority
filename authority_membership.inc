<?php

function authority_membership(&$form_state, $pid) {

  module_load_include('inc', 'islandora_book', 'book_pack_utils');
  $potential_collections = get_collections_as_option_array();
  $current_collections = get_collection_relationships($pid);

  $values = array_values($current_collections);
  $collection_value = $values[0];
  $keys = array_keys($current_collections);
  $collection_key = $keys[0];


  unset($potential_collections[$collection_key]);
  $values = array_values($current_collections);
  $form = array();
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );
  $form['current_collection'] = array(
    '#type' => 'hidden',
    '#value' => $collection_key,
  );
  $form['collections'] = array(
    '#type' => 'fieldset',
    '#collapsed' => true,
    '#collapsible' => true,
    '#title' => t('Collection Membership'),
  );

  $form['collections']['collection_add'] = array(
    '#type' => 'fieldset',
    '#collapsed' => true,
    '#collapsible' => true,
    '#title' => t('Add This Authority to Another Collection'),
  );
  $form['collections']['collection_add']['new_collection'] = array(
    '#title' => 'New Collection',
    '#description' => t("Add a to new collection"),
    '#type' => 'select',
    '#options' => $potential_collections,
  );

  $form['collections']['collection_add']['submit'] = array(
    '#type' => 'submit',
    '#id' => 'add_collection',
    '#value' => t('Add Authority to New Collection'),
  );

  if (count($current_collections) > 1) {
    $form['collections']['collection_remove'] = array(
      '#type' => 'fieldset',
      '#collapsed' => true,
      '#collapsible' => true,
      '#title' => t('Remove Authority from Collection'),
    );

    $form['collections']['collection_remove']['current'] = array(
      '#title' => 'Current Memberships',
      '#type' => 'select',
      '#options' => $current_collections,
    );

    $form['collections']['collection_remove']['submit'] = array(
      '#type' => 'submit',
      '#id' => 'remove_collection',
      '#value' => t('Remove Authority from This Collection'),
    );
  }
  return $form;
}


function authority_membership_submit($form, &$form_state) {
  global $user;
  $pid = $form_state['values']['pid'];
  $collection_pids = get_collection_from_pid($pid);
  $item = new Fedora_Item($pid);

  if ($form_state['clicked_button']['#id'] == 'add_collection') {
    $new_collection = $form_state['values']['new_collection'];
    $item->add_relationship('isMemberOfCollection', $new_collection, RELS_EXT_URI);
    drupal_goto("fedora/repository/$new_collection");
  }

  if ($form_state['clicked_button']['#id'] == 'remove_collection') {
    $current = $form_state['values']['current'];
    $item->purge_relationship('isMemberOfCollection', $current);
    drupal_goto("fedora/repository/$book_pid");
  }

  drupal_goto('fedora/repository/' . $collection_pids[0]);
}
