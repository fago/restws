<?php
// $Id$

/**
 * @file RESTful web services module integration for entities.
 */


/*
 * Controller for entity-bases resources.
 *
 * @todo: Add an interface.
 */
class RestWSEntityResourceController {

  protected $entityType, $entityInfo;

  public function __construct($name, $info) {
    $this->entityType = $name;
    $this->entityInfo = entity_get_info($name);
  }

  /**
   * Returns the property info for the given resource.
   */
  public function propertyInfo() {
    return entity_get_property_info($this->entityType);
  }

  /**
   * Returns a metadata wrapper for the resource with the given id.
   */
  public function wrapper($id) {
    return entity_metadata_wrapper($this->entityType, $id);
  }

  /**
   * Creates a new resource with values matching the property info.
   */
  public function create(array $values) {
    return entity_property_values_create_entity($this->entityType, $values);
  }

  /**
   * Updates a resource with values matching the property info.
   */
  public function update($id, $values) {
    $wrapper = $this->wrapper($id);
    foreach ($values as $name => $value) {
      $wrapper->name->set($value);
    }
    $wrapper->save();
  }

  /**
   * Deletes a resource with the given id.
   */
  public function delete($id) {
    entity_delete($this->entityType, $id);
  }

  /**
   * Determines access for a given resource.
   *
   * @see entity_access()
   */
  public function access($op, $id = NULL) {
    return entity_access($op, $this->entityType, isset($id) ? $this->wrapper($id)->value() : NULL);
  }
}
