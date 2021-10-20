#include "dice_neural_network.h"
/*
#include "math.h"

// Activation function.
static float act(const float x)
{
  if (x < -2.5)
	{
		return 0.0f;
	}
	if (x > 2.5)
	{
		return 1.0f;
	}

	return ((0.2f * x) + 0.5f);
}

static void fprop_layer(const layer_connection lc, const uint16_t output_size, float* output, const float* weights, const float* values) {
	float node_value = 0;
	uint16_t output_counter = 0;

	for (uint16_t j = 0; j < output_size; j++) {
		output_counter = 0;
		for (uint16_t i = 0; i < (lc.length*output_size); i += output_size)
		{
			node_value += (weights[j + i] * values[output_counter]);
			output_counter++;
		}
		output[j]  = act(node_value + lc.bias);
		node_value = 0;
	}

}

static void get_weights(float* weights, uint16_t connection, network nn) {
	uint16_t start = 0;

	for (uint16_t j = 0; j < connection; j++){ start += (LAYER_SIZE[j] * LAYER_SIZE[j+1]); }

	for (uint16_t j = start; j < ((LAYER_SIZE[connection] * LAYER_SIZE[connection + 1]) + start); j++) { weights[(j - start)] = WEIGHTS[j]; }
}

static void set_values(float* out, const float* in, uint16_t size) {
	for (uint16_t j = 0; j < size; j++)
	{
		out[j] = in[j];
	}
}




void predict(float* output, const float* input) {
	float buffer[MAX_NOTES_IN_LAYER];
	float weigths[BIGGEST_CONNECTION];
	float values[MAX_NOTES_IN_LAYER];
	uint16_t i = 0;

	set_values(values, input, LAYER_SIZE[0]);

	for (i = 0; i < (NETWORK_SIZE-2); i++)
	{
		get_weights(weigths, i, nn);
		fprop_layer(nn.layer_connections[i], LAYER_SIZE[i+1], buffer, weigths, values);
		set_values(values, buffer, LAYER_SIZE[i + 1]);
	}

	get_weights(weigths, i, nn);
	fprop_layer(nn.layer_connections[i], LAYER_SIZE[(NETWORK_SIZE - 1)], output, weigths,values);
}

void network_init(void) {
	for (uint16_t i = 0; i < (NETWORK_SIZE-1); i++)
	{
		nn.layer_connections[i].length	= LAYER_SIZE[i];
		nn.layer_connections[i].bias	= BIAS[i];
	}
	
}

*/
