# Native GraphHopper

Calculate offline routes and distances with GraphHopper on the Android
device using a native image of GraphHopper instead of the jar dependency.

The complexity is high and its disadvantage. There are the following
advantages once this works:

 * no strange per-process virtual memory limit
 * GraphHopper does not need to limit itself to Java 7
 * this process works with Gluon and JavaFX
 * same process could work for iOS
 * public transit could work too
 * data import of raw OSM files (especially for small areas) could work too (i.e. on-demand areas and routes could be possible)

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
