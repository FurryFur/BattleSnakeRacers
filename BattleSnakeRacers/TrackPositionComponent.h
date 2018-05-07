#pragma once

class Entity;

struct TrackPositionComponent {
	float distanceAlongTrack; // TODO: Implement a system to update this
	Entity* currentTrackTile;
};