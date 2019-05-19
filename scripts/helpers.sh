#!/bin/bash

function agr_batch_select()
{
    local -n answer_ref=$1

    if [ ! -z "$AGR_ASSUME_YES" ]; then
        answer_ref=$2
        return
    fi

    select selected_item in "${@:2}"; do
        answer_ref=$selected_item
        break
    done
}
