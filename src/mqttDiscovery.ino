#include "balboa_helper.h"

void mqttDiscovery()
{

String Payload;
  if (HASSIO)
  {

    // clear topics:
    mqtt.publish((discoveryTopic + "binary_sensor/Spa").c_str(), "");
    mqtt.publish((discoveryTopic + "sensor/Spa").c_str(), "");
    mqtt.publish((discoveryTopic + "switch/Spa").c_str(), "");
    mqtt.publish("/Spa", "");

    // temperature -> can we try and remove the Payload below, it's messy
    Payload = "{\"name\":\"Hot tub "
              "status\",\"uniq_id\":\"ESP82Spa_1\",\"stat_t\":\"/" +
              mqttTopic +
              "node/"
              "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"],"
              "\"name\":\"Esp Spa\",\"sw\":\"" +
              String(VERSION) + "\"}}";
    mqtt.publish((discoveryTopic + "binary_sensor/Spa/state/config").c_str(), Payload.c_str(), true);
    // climate temperature
    if (SpaConfig.temp_scale == 0)
    {
      mqtt.publish(
          (discoveryTopic + "climate/Spa/temperature/config").c_str(),
          "{\"name\":\"Hot tub "
          "thermostat\",\"uniq_id\":\"ESP82Spa_0\",\"temp_cmd_t\":\"Spa/"
          "target_temp/set\",\"mode_cmd_t\":\"Spa/heat_mode/"
          "set\",\"mode_stat_t\":\"Spa/heat_mode/state\",\"temp_unit\": "
          "\"F\",\"curr_temp_t\":\"Spa/temperature/"
          "state\",\"temp_stat_t\":\"Spa/target_temp/"
          "state\",\"min_temp\":\"80\",\"max_temp\":\"105\",\"modes\":[\"off\","
          " \"heat\"], "
          "\"temp_step\":\"1\",\"platform\":\"mqtt\",\"dev\":{\"ids\":["
          "\"ESP82Spa\"]}}",
          true);
    }
    else if (SpaConfig.temp_scale == 1)
    {
      mqtt.publish(
          (discoveryTopic + "climate/Spa/temperature/config").c_str(),
          "{\"name\":\"Hot tub "
          "thermostat\",\"uniq_id\":\"ESP82Spa_0\",\"temp_cmd_t\":\"Spa/"
          "target_temp/set\",\"mode_cmd_t\":\"Spa/heat_mode/"
          "set\",\"mode_stat_t\":\"Spa/heat_mode/state\",\"temp_unit\": "
          "\"C\",\"curr_temp_t\":\"Spa/temperature/"
          "state\",\"temp_stat_t\":\"Spa/target_temp/"
          "state\",\"min_temp\":\"27\",\"max_temp\":\"40\",\"modes\":[\"off\", "
          "\"heat\"], "
          "\"temp_step\":\"0.5\",\"platform\":\"mqtt\",\"dev\":{\"ids\":["
          "\"ESP82Spa\"]}}",
          true);
    }
    // heat mode
    mqtt.publish(
        (discoveryTopic + "switch/Spa/heatingmode/config").c_str(),
        "{\"name\":\"Hot tub heating "
        "mode\",\"uniq_id\":\"ESP82Spa_3\",\"cmd_t\":\"Spa/heatingmode/"
        "set\",\"stat_t\":\"Spa/heatingmode/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);
    // heating state
    mqtt.publish(
        (discoveryTopic + "binary_sensor/Spa/heatstate/config").c_str(),
        "{\"name\":\"Hot tub heating "
        "state\",\"uniq_id\":\"ESP82Spa_6\",\"stat_t\":\"Spa/heatstate/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);
    // high range
    mqtt.publish(
        (discoveryTopic + "switch/Spa/highrange/config").c_str(),
        "{\"name\":\"Hot tub high "
        "range\",\"uniq_id\":\"ESP82Spa_4\",\"cmd_t\":\"Spa/highrange/"
        "set\",\"stat_t\":\"Spa/highrange/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);

    // OPTIONAL ELEMENTS
    if (SpaConfig.circ)
    {
      // circulation pump
      mqtt.publish(
          (discoveryTopic + "binary_sensor/Spa/circ/config").c_str(),
          "{\"name\":\"Hot tub circulation "
          "pump\",\"uniq_id\":\"ESP82Spa_5\",\"device_class\":\"power\",\"stat_"
          "t\":\"Spa/circ/"
          "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
          true);
    }
    if (SpaConfig.light1)
    {
      // light 1
      mqtt.publish(
          (discoveryTopic + "switch/Spa/light/config").c_str(),
          "{\"name\":\"Hot tub "
          "light\",\"uniq_id\":\"ESP82Spa_7\",\"cmd_t\":\"Spa/light/"
          "set\",\"stat_t\":\"Spa/light/"
          "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
          true);
    }
    if (SpaConfig.pump1 != 0)
    {
      // jets 1
      mqtt.publish(
          (discoveryTopic + "switch/Spa/jet_1/config").c_str(),
          "{\"name\":\"Hot tub "
          "jet1\",\"uniq_id\":\"ESP82Spa_8\",\"cmd_t\":\"Spa/jet_1/"
          "set\",\"stat_t\":\"Spa/jet_1/"
          "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
          true);
    }
    if (SpaConfig.pump2 != 0)
    {
      // jets 2
      mqtt.publish(
          (discoveryTopic + "switch/Spa/jet_2/config").c_str(),
          "{\"name\":\"Hot tub "
          "jet2\",\"uniq_id\":\"ESP82Spa_9\",\"cmd_t\":\"Spa/jet_2/"
          "set\",\"stat_t\":\"Spa/jet_2/"
          "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
          true);
    }
    if (SpaConfig.blower)
    {
      // blower
      mqtt.publish(
          (discoveryTopic + "switch/Spa/blower/config").c_str(),
          "{\"name\":\"Hot tub "
          "blower\",\"uniq_id\":\"ESP82Spa_10\",\"cmd_t\":\"Spa/blower/"
          "set\",\"stat_t\":\"Spa/blower/"
          "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
          true);
    }

    mqtt.publish(
        (discoveryTopic + "sensor/Spa/filter1_start/config").c_str(),
        "{\"name\":\"Filter 1 start\",\"val_tpl\": "
        "\"{{value_json.start}}\",\"uniq_id\":\"ESP82Spa_11\",\"stat_t\":\"Spa/"
        "filter1/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);
    mqtt.publish(
        (discoveryTopic + "sensor/Spa/filter2_start/config").c_str(),
        "{\"name\":\"Filter 2 start\",\"val_tpl\": "
        "\"{{value_json.start}}\",\"uniq_id\":\"ESP82Spa_12\",\"stat_t\":\"Spa/"
        "filter2/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);
    mqtt.publish(
        (discoveryTopic + "sensor/Spa/filter1_duration/config").c_str(),
        "{\"name\":\"Filter 1 duration\",\"val_tpl\": "
        "\"{{value_json.duration}}\",\"uniq_id\":\"ESP82Spa_13\",\"stat_t\":"
        "\"Spa/filter1/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);
    mqtt.publish(
        (discoveryTopic + "sensor/Spa/filter2_duration/config").c_str(),
        "{\"name\":\"Filter 2 duration\",\"val_tpl\": "
        "\"{{value_json.duration}}\",\"uniq_id\":\"ESP82Spa_14\",\"stat_t\":"
        "\"Spa/filter2/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);
    mqtt.publish(
        (discoveryTopic + "binary_sensor/Spa/filter2_enabled/config").c_str(),
        "{\"name\":\"Filter 2 "
        "enabled\",\"uniq_id\":\"ESP82Spa_15\",\"stat_t\":\"Spa/"
        "filter2_enabled/"
        "state\",\"platform\":\"mqtt\",\"dev\":{\"ids\":[\"ESP82Spa\"]}}",
        true);
  }
}