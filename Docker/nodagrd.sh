#!/bin/sh
cd /opt/agrio/bin

if [ -f '/opt/agrio/bin/data-dir/config.ini' ]; then
    echo
  else
    cp /config.ini /opt/agrio/bin/data-dir
fi

if [ -d '/opt/agrio/bin/data-dir/contracts' ]; then
    echo
  else
    cp -r /contracts /opt/agrio/bin/data-dir
fi

while :; do
    case $1 in
        --config-dir=?*)
            CONFIG_DIR=${1#*=}
            ;;
        *)
            break
    esac
    shift
done

if [ ! "$CONFIG_DIR" ]; then
    CONFIG_DIR="--config-dir=/opt/agrio/bin/data-dir"
else
    CONFIG_DIR=""
fi

exec /opt/agrio/bin/nodagr $CONFIG_DIR "$@"
