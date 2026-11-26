#!/bin/sh
# The critic: the same leg on Linux arm64, once the x86-64 container is gone
# (one container at a time).
L=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-critic-scratch/r2/leg
while docker ps --format '{{.Names}}' | grep -q '^p261-critic$'; do sleep 10; done
docker run --rm --pull never --name p261-critic -v $L/route-leg.tar:/src/route-leg.tar:ro -v $L/cases.sh:/src/cases.sh:ro -v $L/leg.sh:/src/leg.sh:ro -v $L/arm64out:/out heroes-linux-arm64:latest sh /src/leg.sh > $L/arm64.docker.log 2>&1
echo "docker exit $?" >> $L/arm64.docker.log
