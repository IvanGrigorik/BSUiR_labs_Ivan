package com.crossplatform.lab1.Controllers;

import com.crossplatform.lab1.Entities.RandomableEntities;
import com.crossplatform.lab1.Logic.RandomService;
import com.crossplatform.lab1.MyLogger;
import com.crossplatform.lab1.Service.Counter;
import lombok.AllArgsConstructor;
import org.apache.logging.log4j.Level;
import org.jetbrains.annotations.NotNull;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;


@Controller
@AllArgsConstructor
public class HomeController {
    RandomService randomService;
    Counter counter;

    @GetMapping("/random")
    public String controllerGet(@RequestParam(value = "num") long number,
                                @RequestParam(value = "md") RandomableEntities.MODES random_mode, @NotNull Model model) {

        // If random_mode == 0 - random less, 1 - random more
        RandomableEntities newEntity = new RandomableEntities(number, random_mode);
        Long result = randomService.generateRandomNumber(newEntity);

        model.addAttribute("num", number);
        model.addAttribute("md", random_mode);
        model.addAttribute("rnum", result);

        counter.incrementCount();

        MyLogger.setLog(Level.INFO, "Successful mapping");

        return "front";
    }

    @GetMapping("/count")
    public String controllerCount(Model model) {
        model.addAttribute("count", counter.getCount());
        return "count";
    }

    @PostMapping("/random1")
    public ResponseEntity<?> controllerPost(@RequestBody ArrayList<RandomableEntities> randomableEntitiesArrayList,
                                            @NotNull Model model) {
        counter.incrementCount();
        MyLogger.setLog(Level.INFO, "Successful controller Post");
        return new ResponseEntity<>(randomService.generateRandomList(randomableEntitiesArrayList), HttpStatus.OK);
    }
}
