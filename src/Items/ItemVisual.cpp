#include "ItemVisual.hpp"

// 30.0f^2
float ItemVisual::high_distance = 900.0f;
// 80.0f^2
float ItemVisual::middle_distance = 6400.0f;
// 160.0f^2
float ItemVisual::low_distance = 25600.0f;

float ItemVisual::grid_size = 40.0f;
int ItemVisual::grid_check_size = 4;
std::map<Vector3i, std::forward_list<ItemVisual*>> ItemVisual::item_grid;
RenderingServer* ItemVisual::rs = nullptr;

void ItemVisual::update_visuals() {
    Vector3 pos = Vector3();

    int count = 0;
    int count_middle = 0;
    int count_low = 0;

    for (int i = -grid_check_size; i <= grid_check_size; i++) {
        for (int j = -grid_check_size; j <= grid_check_size; j++) {
            for (int k = -grid_check_size; k <= grid_check_size; k++) {
                const std::forward_list<ItemVisual*> visuals = item_grid[Vector3i(i, j, k)];

                for (const ItemVisual* visual : visuals) {
                    float distance = pos.distance_squared_to(visual->position);
                    Item* item = visual->item;

                    if (visual->item->use_lod == false) {
                        if (distance <= low_distance) {
                            if (count >= item->multimeshes.size())
                                item->add_multimesh();

                            Item::MultimeshStruct& multimesh = item->multimeshes[count];
                            int index = multimesh.count * 12;

                            multimesh.item_buffer[index +  3] = visual->position.x;
                            multimesh.item_buffer[index +  7] = visual->position.y;
                            multimesh.item_buffer[index + 11] = visual->position.z;
                            multimesh.count++;

                            if (multimesh.count >= item->max_mesh_count)
                                count++;
                        }
                    }
                    else {
                        if (distance <= high_distance) {
                            if (count >= item->multimeshes.size())
                                item->add_multimesh();

                            Item::MultimeshStruct& multimesh = item->multimeshes[count];
                            int index = multimesh.count * 12;

                            multimesh.item_buffer[index +  3] = visual->position.x;
                            multimesh.item_buffer[index +  7] = visual->position.y;
                            multimesh.item_buffer[index + 11] = visual->position.z;
                            multimesh.count++;

                            if (multimesh.count >= item->max_mesh_count)
                                count++;
                        }
                        else if (distance <= middle_distance) {
                            if (count_middle >= item->multimeshes.size())
                                item->add_multimesh();

                            Item::MultimeshStruct& multimesh = item->multimeshes[count];
                            int index = multimesh.count_middle * 12;

                            multimesh.item_buffer_middle[index +  3] = visual->position.x;
                            multimesh.item_buffer_middle[index +  7] = visual->position.y;
                            multimesh.item_buffer_middle[index + 11] = visual->position.z;
                            multimesh.count_middle++;

                            if (multimesh.count_middle >= item->max_mesh_count_middle)
                                count_middle++;
                        }
                        else if (distance <= low_distance) {
                            if (count_low >= item->multimeshes.size())
                                item->add_multimesh();

                            Item::MultimeshStruct& multimesh = item->multimeshes[count];
                            int index = multimesh.count_low * 12;

                            multimesh.item_buffer_low[index +  3] = visual->position.x;
                            multimesh.item_buffer_low[index +  7] = visual->position.y;
                            multimesh.item_buffer_low[index + 11] = visual->position.z;
                            multimesh.count_low++;

                            if (multimesh.count_low >= item->max_mesh_count_low)
                                count_low++;
                        }
                    }
                }
            }
        }
    }

    // finally push updates to RenderingServer
    for (auto it = Item::item_instances.begin(); it != Item::item_instances.end(); ++it) {
        const Item* item = it->second;

        for (Item::MultimeshStruct& multimesh : it->second->multimeshes) {
            rs->multimesh_set_buffer(multimesh.multimesh, multimesh.item_buffer);
            rs->multimesh_set_visible_instances(multimesh.multimesh, multimesh.count);

            if (it->second->use_lod == false)
                break;
            
            rs->multimesh_set_buffer(multimesh.multimesh_middle, multimesh.item_buffer_middle);
            rs->multimesh_set_visible_instances(multimesh.multimesh_middle, multimesh.count_middle);

            rs->multimesh_set_buffer(multimesh.multimesh_low, multimesh.item_buffer_low);
            rs->multimesh_set_visible_instances(multimesh.multimesh_low, multimesh.count_low);
        }
    }
}

void ItemVisual::set_position(const Vector3& new_position) {
    position = new_position;
    Vector3i new_grid_position = (Vector3i)(position / grid_size);

    if (grid_position != new_grid_position) {
        item_grid[grid_position].remove(this);
        item_grid[new_grid_position].push_front(this);
        grid_position = new_grid_position;
    }
}