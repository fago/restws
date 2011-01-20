<?php
// $Id$

/**
 * @file RESTful web services module formats.
 */

/**
 * Interface implemented by formatter implementations for the http client
 */
interface RestWSFormatInterface {

  public function __construct($name, $info);

  // TODO: add CRUD resource functions.
}

/**
 * A base for all simple formats that are just serializing/unserializing values.
 */
abstract class RestWSBaseFormat implements RestWSFormatInterface {

  public function __construct($name, $info) {

  }

  /**
   * Gets the representation of a resource.
   */
  public function viewResource($resourceController, $id) {
    $wrapper = $resourceController->wrapper($id);
    return $this->serialize(self::getData($wrapper));
  }

  /**
   * Creates a new resource.
   */
  public function createResource($resource, $data) {
    $values = $this->unserialize($data);
    $resourceController->create($values);
  }

  /**
   * Updates a resource.
   */
  public function updateResource($resource, $id, $data) {
    $values = $this->unserialize($data);
    $resourceController->update($id, $values);
  }

  /**
   * Deletes a resource.
   */
  public function deleteResource($resource, $id) {
    $resourceController->delete($id);
  }



  /**
   * Gets a simple PHP array using URI references for some wrapped data.
   */
  public static function getData($wrapper) {
    $data = array();
    foreach ($wrapper as $name => $property) {
      try {
        if ($property instanceof EntityDrupalWrapper) {
          // For referenced entities only return the URI.
          if ($id = $property->getIdentifier()) {
            $data[$name] = array('uri' => restws_resource_uri($property->type(), $id));
          }
        }
        elseif ($property instanceof EntityValueWrapper) {
          $data[$name] = $property->value();
        }
        elseif ($property instanceof EntityListWrapper || $property instanceof EntityStructureWrapper) {
          $data[$name] = self::getData($property);
        }
      }
      catch (EntityMetadataWrapperException $e) {
        // A property causes problems - ignore that.
      }
    }
    return $data;
  }

  /**
   * Serializes arbitrary data values.
   *
   * @param array $values
   *  The data that should be serialized.
   *
   * @return
   *  The serialized data as a string.
   */
  abstract public function serialize($values);

  /**
   * Unserializes data.
   *
   * @param string $data
   *  The data that should be unserialized.
   *
   * @return
   *  The unserialized data.
   */
  abstract function unserialize($data);

  /**
   * Returns the mime type to use.
   */
  abstract function mimeType();

}

/**
 * A base formatter to format json.
 */
class RestWSFormatJSON extends RestWSBaseFormat {

  protected $mimeType = 'application/json';

  public function serialize($values) {
    return drupal_json_encode($values);
  }

  public function unserialize($data) {
    return drupal_json_decode($data);
  }

  public function mimeType() {
    return $this->mimeType;
  }
}
