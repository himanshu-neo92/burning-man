////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Scene containing a box with octet.
  #define height_image 256
  #define width_image 256
  class buring_man : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
    particle_sys system;
    struct my_vertex {
      vec3p pos;
      vec3p nor;
      uint32_t color;
    };
    static uint32_t make_color(float r, float g, float b) {
      return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
    }    

    void mouse_control(mat4t &camera){

      int x, y;
      get_mouse_pos(x, y);
      int vx, vy;
      get_viewport_size(vx, vy);

      mat4t modelToWorld;

      modelToWorld.loadIdentity();
      modelToWorld[3] = vec4(camera.w().x(), camera.w().y(), camera.w().z(), 1);
      modelToWorld.rotateY((float)-x*2.0f);
      if (vy / 2 - y < 70 && vy / 2 - y > -70)
        modelToWorld.rotateX((float)vy / 2 - y);
      if (vy / 2 - y >= 70)
        modelToWorld.rotateX(70);
      if (vy / 2 - y <= -70)
        modelToWorld.rotateX(-70);
      camera = modelToWorld;//apply to the node
    }

    void key_presses(mat4t &camera){

      if (is_key_down(key_esc)){
        exit(0);
      }

      //camera movement keys (arrow keys and WASD)
      if (is_key_down(key::key_shift))
      {
        camera.translate(0, 5, 0);
      }
      if (is_key_down(key::key_ctrl))
      {
        camera.translate(0, -5, 0);
      }
      if (is_key_down('W'))
      {
        camera.translate(0, 0, -5);
      }
      if (is_key_down('S'))
      {
        camera.translate(0, 0, 5);
      }
      if (is_key_down('A'))
      {
        camera.translate(-5, 0, 0);
      }
      if (is_key_down('D'))
      {
        camera.translate(5, 0, 0);
      }
    }
  public:
    /// this is called when we construct the class before everything is initialised.
    buring_man(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->set_far_plane(10000);
      mat4t &camera_mat = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      camera_mat.translate(0, 50, 0);
      camera_mat.rotateY(-180);
      camera_mat.rotateX(0);

      
      system.Init(1,app_scene);

      particle_emitter *em =new particle_emitter(vec3(),vec3(0,1,0),1,1,1,50);

      system.AddEmitter(em);
     

      
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);
      
      system.Update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);
      mat4t &cam=app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      mouse_control(cam);
      key_presses(cam);
    }
  };
}
