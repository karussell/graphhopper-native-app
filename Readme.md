# Native GraphHopper

## Get Started

### Build the Graph Files

 * Get the repository https://github.com/karussell/graphhopper-native-image/
 * mvn clean install
 * download a pbf file from OSM e.g. `wget http://download.geofabrik.de/europe/germany/berlin-latest.osm.pbf`
 * `ln -s berlin-latest.osm.pbf osm.pbf`
 * `java -jar target/graphhoppernative-1.0-SNAPSHOT-jar-with-dependencies.jar`
 * `adb push graph-cache/ /data/local/tmp/`

### Build the Library

Follow instructions in https://github.com/karussell/graphhopper-native-image/pull/2

Copy the created `libgraphhoppernative.so` into `distribution/ghlib/arm64-v8a/`
